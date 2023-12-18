#______________________________________________________________________________#
############################### Target Names ###################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

# Name

PROJECT_NAME = ircserv
NAME	= ./$(PROJECT_NAME)

#______________________________________________________________________________#
############################### Compiler #######################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

# Compiler
CC = /usr/bin/c++

# Compiler Flags
CFLAGS = -Wall -Wextra -Werror -std=c++98
CFLAGS +=	-MMD -MP
CFLAGS +=	-Wshadow
CFLAGS += -g

#______________________________________________________________________________#
############################### Print Variables ################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

# Reset
NC = \033[0m

# Colors
YELLOW = \033[0;33m
GREEN = \033[0;32m
BLUE = \033[0;34m
RED = \033[0;31m
PURPLE = \033[0;35m
CYAN = \033[0;36m
BLACK = \033[0;30
WHITE = \033[0;37m

# Colors
BYELLOW = \033[1;33m
BGREEN = \033[1;32m
BBLUE = \033[1;34m
BRED = \033[1;31m
BPURPLE = \033[1;35m
BCYAN = \033[1;36m
BBLACK = \033[1;30m
BWHITE = \033[1;37m

# One Line Output
OL =\e[1A\r\033[K
CLEAR = \033[2K

#______________________________________________________________________________#
############################### Dependencies ###################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

DEPS_DIR =	srcs

DEPS += -I $(DEPS_DIR)

#______________________________________________________________________________#
############################### Headers ########################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

HEADER +=	Server.hpp\
			Client.hpp\
			Channel.hpp\
			Command.hpp

vpath %.hpp $(DEPS_DIR)

#______________________________________________________________________________#
############################### Path Sources ###################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

SRCS_DIR += srcs\
			srcs/Bot\
			srcs/Server\
			srcs/Channel\
			srcs/Client\
			srcs/Command

PATH_SRCS = $(SRCS_DIR)

#______________________________________________________________________________#
############################### Sources ########################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

SRCS +=	main.cpp\
		Bot.cpp\
		Server.cpp\
		Channel.cpp\
		Client.cpp\
		Command.cpp
#______________________________________________________________________________#
############################### Attribution ####################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

vpath %.cpp $(PATH_SRCS)

#______________________________________________________________________________#
############################### Objects ########################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

OBJS_DIR = ./objs
OBJS = $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(SRCS))
CHK_OBJS = $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(CHK_SRCS))

#______________________________________________________________________________#
############################### Build ##########################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

BUILD_DIR = ./build
BUILD = $(addprefix $(BUILD_DIR)/, $(notdir $(OBJS:.o=.d)))

#______________________________________________________________________________#
############################### Progress Bar ###################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

# PROGRESS BAR
NB_OBJ = ${words ${OBJS}}
COUNTER = 1
PROGRESS = 0
DONE = 100
SPACE = 0
FILL = 0
EMPTY = 0

define PROGRESS_BAR
	$(eval PROGRESS=$(shell echo $$(($(COUNTER) * 100 / $(NB_OBJ)))))
	$(eval DONE=$(shell echo $$(($(PROGRESS) * 30 / 100))))
	$(eval SPACE=$(shell echo $$((30 - $(DONE)))))
	$(eval FILL=$(shell printf "%*s" ${DONE} | sed 's/ /◼/g'))
	$(eval EMPTY=$(shell printf "%*s" ${SPACE} | sed 's/ /◼/g'))
	printf "\r${CLEAR}$(BCYAN)Compile : $(GREEN)${FILL}$(RED)${EMPTY} $(BWHITE)%3d%%$(END) $(CYAN)%s.c " ${PROGRESS} $1
	$(eval COUNTER=$(shell echo $$(($(COUNTER) + 1))))
endef

#______________________________________________________________________________#
############################### Rules ##########################################
#‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾#

#_____Build_____#
all:	$(NAME)
		@echo "--▶ $(BGREEN)all\t$(GREEN)done$(NC)"
		@echo "--------------------------------------------------"

$(NAME):	$(OBJS)
		@$(CC) $(CFLAGS) $^ $(LDFLAGS) $(DEPS) -o $@
    	#-----Output-----#
		@echo "$(WHITE)\n▶$(BGREEN)$(NAME)\t$(GREEN)Executable created$(NC)"

$(OBJS): 	$(OBJS_DIR)/%.o: %.cpp | dir where
			@$(CC) $(CFLAGS) -MMD $(DEPS) -c $< -o $@
			@mv $(basename $@).d $(BUILD_DIR)/$(notdir $(basename $@)).d
    		#-----Output-----#
			@$(call PROGRESS_BAR, $(basename $(notdir $<)))

-include $(BUILD)

# Directories
dir: $(OBJS_DIR) $(BUILD_DIR)

$(OBJS_DIR):
	@mkdir $@

$(BUILD_DIR):
	@mkdir $@

bonus:	all

#_____Clean_____#
clean:	where_c
		@rm -rf $(OBJS_DIR)
		@rm -rf $(BUILD_DIR)
    	#-----Output-----#
		@echo "▶ $(BYELLOW)clean $(YELLOW)Object files removed from $(PROJECT_NAME)$(NC)"
		@echo "▶ \t$(YELLOW)Dependencies files removed from $(PROJECT_NAME)$(NC)"
		@echo "--▶ $(BYELLOW)clean$(YELLOW) done$(NC)"

fclean:	clean
		@rm -f $(NAME)
    	#-----Output-----#
		@echo "▶ $(BRED)fclean $(RED)libft.a removed$(NC)"
		@echo "▶ $(BRED)fclean $(RED)libmlx.a removed$(NC)"
		@echo "\t$(RED)$(NAME) removed$(NC)"
		@echo "--▶ $(BRED)fclean$(RED) done$(NC)"

# Current Make
where_c:
	@echo "$(BBLACK)>$(PROJECT_NAME)$(NC)"

where:
	@echo "$(BBLACK)>$(PROJECT_NAME)$(NC)"

re:	fclean all

.PHONY: all clean fclean re where where_c dir bonus
