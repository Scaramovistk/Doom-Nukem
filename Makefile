# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gscarama <gscarama@student.s19.be>         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/17 11:38:26 by gscarama          #+#    #+#              #
#    Updated: 2024/07/17 12:12:40 by rperez-t         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

include mk/config.mk
include mk/sources.mk

# Rules
all:			$(NAME)

$(NAME):		$(LIBFT) $(MLX_FILE) $(OBJ)
				$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

$(LIBFT):
				$(MAKE) --no-print-directory -C $(LIBFT_DIR)

$(MLX_FILE):
				@$(MAKE) --no-print-directory -sC $(MLX_DIR)

build/%.o:		src/%.c
				@mkdir -p $(dir $@)
				$(CC) $(CFLAGS) -c $< -o $@

clean:
				@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
				@$(MAKE) --no-print-directory -sC $(MLX_DIR) clean
				$(RM) build

fclean:			clean
				$(RM) $(NAME)
				$(RM) ./tests/maps/
				@mkdir -p ./tests/maps/
				@touch ./tests/maps/hold.txt
				@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean

re:				fclean all

maps:			$(NAME)
				@set -e; for map in tests/maps_src/*.cub; do \
					output="tests/maps/$$(basename "$$map" .cub).dnk"; \
					./$(NAME) --pack "$$map" "$$output"; \
				done

-include $(DEP)

.PHONY:			all clean fclean re maps
