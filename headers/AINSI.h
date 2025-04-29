
#ifndef FT_MALCOM_H
# define FT_MALCOM_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/*
** ==================== ANSI ESCAPE COLORS ====================
*/

# define RESET      "\033[0m"
# define RED        "\033[31m"
# define GREEN      "\033[32m"
# define YELLOW     "\033[33m"
# define BLUE       "\033[34m"
# define MAGENTA    "\033[35m"
# define CYAN       "\033[36m"
# define WHITE      "\033[37m"
# define BOLD       "\033[1m"
# define UNDERLINE  "\033[4m"

/*
** ================ FT_MALCOM PROJECT MACROS ==================
*/

# define LOG_INFO(msg)     printf(GREEN "[INFO] " RESET "%s\n", msg)
# define LOG_WARN(msg)     printf(YELLOW "[WARNING] " RESET "%s\n", msg)
# define LOG_ERROR(msg)    printf(RED BOLD "[ERROR] " RESET WHITE BOLD"%s\n"RESET, msg)
# define LOG_DEBUG(msg)    printf(CYAN "[DEBUG] " RESET "%s\n", msg)

#endif
