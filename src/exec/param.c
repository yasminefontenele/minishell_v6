/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:23:23 by emencova          #+#    #+#             */
/*   Updated: 2024/09/23 06:51:43 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/*int check_file_permissions(char *path, int is_write) 
{
    if (access(path, F_OK) == -1) 
    {
        if (!is_write) 
        {
            m_error(ERR_NEWDIR, path, 127);
            return (-1);
        }
    } 
    else 
    {
        if (!is_write && access(path, R_OK) == -1) 
        {
            m_error(ERR_NWPERM, path, 126);
            return (-1);
        }
        if (is_write && access(path, W_OK) == -1) 
        {
            m_error(ERR_NWPERM, path, 126);
            return (-1);
        }
    }
    return (0);
}*/

int check_file_permissions(char *path, int is_write) 
{
    if (access(path, F_OK) == -1) 
    {
        if (!is_write) 
        {
            m_error(ERR_NEWDIR, path, 127);
            return (-1);
        }
    } 
    else 
    {
        if (!is_write) 
        {
            if (!is_write && access(path, R_OK | X_OK) == -1) 
            {
                m_error(ERR_NWPERM, path, 126);
                return (-1);
            }
        }
        if (is_write && access(path, W_OK) == -1) 
        {
            m_error(ERR_NWPERM, path, 126);
            return (-1);
        }
    }
    return (0);
}


int open_file(char *path, int is_write, int is_append) 
{
    if (!path || check_file_permissions(path, is_write) == -1)
        return (-1);
    if (is_write) 
    {
        if (is_append) 
            return (open(path, O_CREAT | O_WRONLY | O_APPEND, 0666)); 
        else
            return (open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666));
    }
    return (open(path, O_RDONLY));
}


void close_old_fd(int oldfd)
{
    if (oldfd > 2)
        close(oldfd);
}

t_exec *get_outfile(t_exec *node, char **args, int *i, int append)
{
    int is_write;
    int is_append;
    char *error_message;

    is_write = 1;
    is_append = append;
    error_message = "minishell: syntax error near unexpected token `newline'";
    (*i)++;
    close_old_fd(node->out);
    if (args[*i])
        node->out = open_file(args[*i],is_write, is_append);
    if (!args[*i] || node->out == -1) 
    {
        *i = -1;
        ft_putendl_fd(error_message, 2);
        if (node->out == -1)
            g_env.exit_status = 1;
        else
            g_env.exit_status = 2;
    }
    return node;
}

t_exec *get_infile(t_exec *node, char **args, int *i, int here_doc)
{
    int is_write;
    int is_append;
    char *error_message;
    
    is_write = 0;
    is_append = 0;
    error_message = "minishell: syntax error near unexpected token `newline'";
    (*i)++;
    close_old_fd(node->in);
    if (here_doc) 
    {
        char *aux[2] = {args[*i], "minishell: warning: here-document delimited by end-of-file"};
        node->in = create_here_document_fd(NULL, aux);
    } else if (args[*i])
        node->in = open_file(args[*i], is_write, is_append);
    if (!args[*i] || node->in == -1)
    {
        *i = -1;
        ft_putendl_fd(error_message, 2);
        if (node->in == -1)
            g_env.exit_status = 1;
        else
            g_env.exit_status = 2;
    }
    return (node);
}
