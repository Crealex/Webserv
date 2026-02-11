NAME	= webServ
SRCS	= $(addprefix srcs/, Server.cpp main.cpp printDebug.cpp Client.cpp Loop.cpp)
PARSING	= $(addprefix srcs/parsing_config/, createStructV2.cpp parseRoot.cpp parseAddressPort.cpp parseErrorPage.cpp parseMaxSize.cpp parseHostname.cpp parseUtils.cpp Location.cpp)
SOCKET  = $(addprefix srcs/socket/, createSocket.cpp Socket.cpp SocketData.cpp)
REQUEST = $(addprefix srcs/request/, methodsUtils.cpp Delete.cpp Get.cpp Method.cpp MimeTypes.cpp Post.cpp Request.cpp ResponseError.cpp)
EPOLL   = $(addprefix srcs/epoll/, Epoll.cpp addEpollFd.cpp)
OBJS	= ${SRCS:%.cpp=${OBJDIR}/%.o}
OBJPARS	= ${PARSING:%.cpp=${OBJDIR}/%.o}
OBJSOCK	= ${SOCKET:%.cpp=${OBJDIR}/%.o}
OBJREQ	= ${REQUEST:%.cpp=${OBJDIR}/%.o}
OBJEPO	= ${EPOLL:%.cpp=${OBJDIR}/%.o}
OBJDIR	= objets
CFLAGS	= -Werror -Wextra -Wall -std=c++98 -g3
CC = c++

# Colors and style
END      := $(shell printf "\033[0m")
WHITE    := $(shell printf "\033[1;37m")
NC       := $(shell printf "\033[0m")
PINK     := $(shell printf "\033[1;35m")
GREEN    := $(shell printf "\033[32m")
BOLD     := $(shell printf "\033[1m")
L_PURPLE := $(shell printf "\033[38;5;55m")
YELLOW   := $(shell printf "\033[33m")
BLUE     := $(shell printf "\033[34m")
BLACK    := $(shell printf "\033[1;90m")

# Counter or compiled
TOTAL_FILES := $(words $(SRCS) $(PARSING) $(SOCKET) $(REQUEST) $(EPOLL))
CURRENT_FILE = 0
all:	${NAME} display_ascii

${NAME}:	${OBJS} ${OBJPARS} ${OBJSOCK} ${OBJREQ} ${OBJEPO}
	@${CC} ${CFLAGS} ${OBJS} ${OBJPARS} ${OBJSOCK} ${OBJREQ} ${OBJEPO} -o ${NAME}
	@echo "${BOLD}${GREEN}📦 Link complete: ${NAME}${END}"

${OBJDIR}/%.o: %.cpp | ${OBJDIR}
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))
	@printf "${BOLD}${L_PURPLE}⚡ [%2d/%2d] Compiling: %-20s ${END}" $(CURRENT_FILE) $(TOTAL_FILES) "$<"
	@${CC} ${CFLAGS} -c -o $@ $<
	@echo "${BOLD}${GREEN}✓${END}"

${OBJDIR}:
	@mkdir -p ${OBJDIR}
	@mkdir -p ${OBJDIR}/srcs
	@mkdir -p ${OBJDIR}/srcs/parsing_config
	@mkdir -p ${OBJDIR}/srcs/socket
	@mkdir -p ${OBJDIR}/srcs/request
	@mkdir -p ${OBJDIR}/srcs/epoll
	@echo "${BOLD}${BLUE}📁 Created objects directory${END}"

clean:
	@echo "${BOLD}${YELLOW}🧹 Cleaning objects...${END}"
	@rm -rf ${OBJDIR}
	@echo "${BOLD}${GREEN}✓ Clean complete${END}"

fclean: clean
	@echo "${BOLD}${YELLOW}🗑️  Deep cleaning...${END}"
	@rm -f ${NAME}
	@rm -f test
	@echo "${BOLD}${GREEN}✨ All clean ✨${END}"

re: fclean all

.PHONY: all clean fclean re display_ascii

testalex: 
	c++ -Werror -Wall -Werror $(addprefix srcs/, request/MethodsClass.cpp parsing_config/createStructV2.cpp request/MimeTypes.cpp parsing_config/Location.cpp parsing_config/pars*.cpp Server.cpp request/Get.cpp request/methodsUtils.cpp request/ResponseError.cpp printDebug.cpp) -o test

display_ascii:
	@echo "$(BOLD)$(BLACK)"
	@if [ -f ascii_art.txt ]; then \
		cat ascii_art.txt; \
	fi
	@echo "$(END)"
	@echo "${BOLD}${L_PURPLE} ✨CPP-Module09-ex00✨ ${GREEN} is ready 🎉 ${END}"
	@echo "${BOLD}${BLUE}➜ Use ./${NAME} to run the program${END}"
