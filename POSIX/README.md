POSIX wrapping lib

Library Description

This library needs to use posix standard function and be able to catch errors, that would be.
Thanks to this lib, you can forget about regular checks successful or failture working a function.
If function failed, throw PosixError with description about error (inherits from std::exception)