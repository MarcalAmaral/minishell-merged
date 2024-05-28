/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_aux_functions.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 19:30:55 by marvin            #+#    #+#             */
/*   Updated: 2024/05/27 19:30:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int files_out_control(t_ast *raiz)
{
    int i;

    i = 0;
    while (raiz->files[1][i] != NULL)
    {
        raiz->r_fds.r_fd_out = open(raiz->files[1][i], O_WRONLY | O_CREAT | O_TRUNC, 0000666);
        i++;
    }
    return (raiz->r_fds.r_fd_out);
}

int files_in_control(t_ast *raiz)
{
    int i;

    i = 0;
    while (raiz->files[0][i] != NULL)
    {
        raiz->r_fds.r_fd_in = open(raiz->files[0][i], O_RDONLY);
        //printf("TESTE = %d\n", raiz->r_fds.r_fd_in);
        i++;
    }
    return (raiz->r_fds.r_fd_in);  
}