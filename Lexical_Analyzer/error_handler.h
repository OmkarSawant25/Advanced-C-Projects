#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

void lexical_error(const char *msg, int line);
void lexical_error_char(char c, int line);
void syntax_error(const char *msg, int line);

#endif
