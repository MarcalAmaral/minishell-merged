/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myokogaw <myokogaw@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:32:57 by myokogaw          #+#    #+#             */
/*   Updated: 2024/04/25 15:34:43 by myokogaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_redirect(enum e_type type)
{
	if (type == R_OUT || type == R_IN || type == DGREAT || type == DGREAT)
		return (1);
	return (0);
}
