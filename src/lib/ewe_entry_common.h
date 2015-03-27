
#ifndef EWE_ENTRY_COMMON_H
#define EWE_ENTRY_COMMON_H

/**
 * @def EWE_REG_EXTENDED
 * @brief Regex check option: Use Extended Regular Expressions.
 */
#define EWE_REG_EXTENDED  1
/**
 * @def EWE_REG_ICASE
 * @brief Regex check option: Ignore case in match.
 */
#define EWE_REG_ICASE     2

/**
 * @def EWE_REG_NOERROR
 * @brief Regex Success: Regex maches to the Entrys text
 */
#define EWE_REG_NOERROR  0
/**
 * @def EWE_REG_NOMATCH
 * @brief Regex Error: regexec() failed to match.
 */
#define EWE_REG_NOMATCH  1
/**
 * @def EWE_REG_BADPAT
 * @brief Regex Error: Invalid regular expression.
 */
#define EWE_REG_BADPAT   2
/**
 * @def EWE_REG_ECOLLATE
 * @brief Regex Error: Invalid collating element referenced.
 */
#define EWE_REG_ECOLLATE 3
/**
 * @def EWE_REG_ECTYPE
 * @brief Regex Error: Invalid character class type referenced.
 */
#define EWE_REG_ECTYPE   4
/**
 * @def EWE_REG_EESCAPE
 * @brief Regex Error: Trailing \ in pattern.
 */
#define EWE_REG_EESCAPE  5
/**
 * @def EWE_REG_ESUBREG
 * @brief Regex Error: Wrong back reference.
 */
#define EWE_REG_ESUBREG  6
/**
 * @def EWE_REG_EBRACK
 * @brief Regex Error: [ ] imbalance.
 */
#define EWE_REG_EBRACK   7
/**
 * @def EWE_REG_EPAREN
 * @brief Regex Error: \( \) or ( ) imbalance.
 */
#define EWE_REG_EPAREN   8
/**
 * @def EWE_REG_EBRACE
 * @brief Regex Error: \{ \} imbalance.
 */
#define EWE_REG_EBRACE   9
/**
 * @def EWE_REG_BADBR
 * @brief Regex Error: Content of \{ \} invalid: not a number, number too large, more than two numbers, first larger than second.
 */
#define EWE_REG_BADBR    10
/**
 * @def EWE_REG_ERANGE
 * @brief Regex Error: Invalid endpoint in range expression.
 */
#define EWE_REG_ERANGE   11
/**
 * @def EWE_REG_ESPACE
 * @brief Regex Error: Out of memory.
 */
#define EWE_REG_ESPACE   12
/**
 * @def EWE_REG_BADRPT
 * @brief Regex Error: ?, * or + not preceded by valid regular expression.
 */
#define EWE_REG_BADRPT   13

#endif /* EWE_ENTRY_COMMON_H */
