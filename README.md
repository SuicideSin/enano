Nano-like clone that reads/writes encrypted files.
- Uses openssl. Creates a random IV and salt upon creation, uses PBKDF2 for key generation.
- Password and plaintext data never touch the harddrive, stay in ram the whole time.
- Uses AES256 encryption.

Untested on Windows...

Wanted features:
- Find text