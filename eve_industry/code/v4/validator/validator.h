/// @file Function prototypes for validation functions

#ifndef HEADER_GUARD_VALIDATOR
#define HEADER_GUARD_VALIDATOR

/// @brief Perform a series of tests on the @ref args_t class
///
/// @return True if all tests completed successfully and false if at least
/// one error occurred.
///
/// @par Side Effects
/// Will print to std::cerr if an error occurs.
bool validate_args();

#endif // Header guard
