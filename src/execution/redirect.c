/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 17:03:17 by marvin            #+#    #+#             */
/*   Updated: 2024/05/24 17:03:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_r_fds r_fds_control(t_ast *raiz, t_pipex *p)
{
    t_r_fds fds;

    if (raiz->files[0] != NULL)
    {
        fds.r_fd_in = files_in_control(raiz);
        p->fd_exec[0] = fds.r_fd_in;
    }
    else
        fds.r_fd_in = 0;    
    if (raiz->files[1] != NULL)
    {
        fds.r_fd_out = files_out_control(raiz);
        p->fd_exec[1] = fds.r_fd_out;
    }
    else
        fds.r_fd_out = 0;
    return (fds);
}

char    **creat_file_mat(t_dlist *tokens, int result, enum e_type type)
{
    t_dlist *aux_t;
    char    **mat;
    int     i;

    i = -1;
    aux_t = tokens;
    if (result > 0)
    {
        mat = ft_calloc(sizeof(char *), (result + 1));
        while (aux_t->next != NULL)
		    aux_t = aux_t->next;
	    while (aux_t->tok->type != PIPE && aux_t->prev != NULL)
            aux_t = aux_t->prev;
        while (aux_t)
        {
            if (aux_t->tok->type == type)
                mat[++i] = ft_strdup(aux_t->next->tok->lex);
            aux_t = aux_t->next;
        }
    }
    else
        mat = NULL;
    return (mat);
}

char    **files_in(t_dlist *tokens)
{
    int     result;
    t_dlist *aux_t;
    char    **files;

    aux_t = tokens;
    result = 0;
    while (aux_t->next != NULL)
		aux_t = aux_t->next;
	while (aux_t->tok->type != PIPE && aux_t->prev)
    {
        if (aux_t->tok->type == R_IN)
            result++;
		aux_t = aux_t->prev;
    }
    if (aux_t->tok->type == R_IN)
        result++;
    files = creat_file_mat(tokens, result, R_IN);
    return (files);
}

char    **files_out(t_dlist *tokens)
{
    int     result;
    t_dlist *aux_t;
    char    **files;

    aux_t = tokens;
    result = 0;
    while (aux_t->next != NULL)
		aux_t = aux_t->next;
	while (aux_t->tok->type != PIPE && aux_t->prev != NULL)
    {
        if (aux_t->tok->type == R_OUT)
            result++;
		aux_t = aux_t->prev;
    }
    files = creat_file_mat(tokens, result, R_OUT);
    return (files);
}



char    ***have_redirect(t_dlist *tokens)
{
    char    ***files;

    files = ft_calloc(sizeof(char **), 3);
    files[0] = files_in(tokens);
    files[1] = files_out(tokens);

    return (files);
}