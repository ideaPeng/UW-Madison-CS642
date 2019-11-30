# Part A:

## Recovered password
13145820

## Pseudocode for attack

Target Hash := T
Target Password := K
Loop over possbile password length L
    Loop over possbile password combination P
        C(P) := Concated result of username, password and salt
        H(C(P)) := SHA-256 Hash of C(P)
        If H(C(P)) = T:
            K = P
            End

## Worst case running time

Worst-case running time will be O(10^L), where L is the max possbile password
length.


## Discussion of current proposal and suggestions for improvement

The choice of the hash function is good and adding salt and username before
doing the hash is also good. However, the password itself is not strong enough,
which make attacker easy to do brute force attack. Thus, I suggest that user 
should be require to set longer and more complex password.

# Part B:

## Discussion of the current scheme
Current encryption can not guarantee the integrity of the data and it allow
the attacker to modify the first block of the message by manipulating the IV
and replace the tag at the condition that they know the plaintext beforehand.

## Suggestions for improving the scheme
The colleague should apply encrypt then MAC method instead of encrypt and MAC
method. That is, it should attach the sha256 digest of the encrypted message
instead of the sha256 digest of plaintext to ensure only people who has key can
modify the message.

# Extra credit: More password cracking

## password
M@dis0n

## pseudocode
Given the human password dictionary D0, Password Requirement filter Fp, 
256time-Iteration SHA256 Hash function H, Salt Wrap Function W, target
SHA256 result T.
Possible human password dictionary Dp := Fp(D0)
For all possilbe password p in Dp:
    If H(W(p)) == T:
       The password should be p.
       break
    
