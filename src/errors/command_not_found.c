/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_not_found.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 17:02:51 by myokogaw          #+#    #+#             */
/*   Updated: 2024/05/01 17:23:11 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_not_found(char *path, char **matrix)
{
	if (path == NULL)
	{
		ft_putstr_fd(matrix[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ft_free_matrix((void **) matrix);
		return (127);
	}
	return (0);
}
