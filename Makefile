NAME        = codexion
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread -Iincludes -MMD -MP
RM          = rm -rf

# Os Diretorios
SRC_DIR     = src
OBJ_DIR     = obj
INC_DIR     = includes

# Lista de fontes
SRCS        = $(SRC_DIR)/main.c

# Mapeamento de objetos e dependencias
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS        = $(OBJS:.o=.d)

# Cores para deixar mais legal ;)
GREEN       = \033[0;32m
YELLOW      = \033[0;33m
RESET       = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compilado com sucesso!$(RESET)"

# Para compilar os .o mantendo a hierarquia de pastas dentro de obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@echo "$(YELLOW)Objetos e dependencias removidos.$(RESET)"

fclean: clean
	@$(RM) $(NAME)
	@echo "$(YELLOW)Executavel $(NAME) removido.$(RESET)"

re: fclean all

# Inclui os arquivos .d para rastrear alteracoes nos headers (.h)
-include $(DEPS)

.PHONY: all clean fclean re