/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_program.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:01:47 by myokogaw          #+#    #+#             */
/*   Updated: 2024/05/20 17:50:40 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	term_properties(int restore)
// {
// 	static struct termios	term;

// 	if (!restore)
// 		tcgetattr(STDIN_FILENO, &term);
// 	else
// 		tcsetattr(STDIN_FILENO, TCSANOW, &term);
// }

int	run_program(void)
{
	t_dlist	**tokens;
	char	*input;
	char	*entrance;

	hook_environ(copy_environ(), 0);
	hook_pwd(catch_cwd(), 0);
	handle_signal();
	while (TRUE)
	{
		entrance = set_entrance();
		after_prompt(0);
		input = readline(entrance);
		after_prompt(1);
		free(entrance);
		add_history(input);
		if (input == NULL)
			return (interrupt_program(input));
		if (quote_validation(input))
			last_exit_status(syntax_error(UNCLOSED_QUOTE, NULL));
		else if (*input != '\0')
		{
			tokens = lexer(input);
			parser(tokens);
			ast_function(tokens);
		}
		free(input);
	}
	return (last_exit_status(-1));
}
