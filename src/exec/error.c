/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 16:43:00 by emencova          #+#    #+#             */
/*   Updated: 2024/09/20 22:49:56 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include <dirent.h>

void m_error(int error_type, char *limit, int status)
{
    if (error_type == ERR_NEWDIR)
        ft_putstr_fd("cd: no such file or directory: ", 2);
    else if (error_type == ERR_NOTDIR)
        ft_putstr_fd("cd: not a directory: ", 2);
    else if (error_type == ERR_NWPERM)
        ft_putstr_fd("cd: permission denied: ", 2);
    else if (error_type == ERR_DUP)
        ft_putstr_fd("dup2 error: ", 2);
    else if (error_type == ERR_FORK)
        ft_putstr_fd("fork error: ", 2);
    else if (error_type == ERR_QUOTE)
		ft_putstr_fd("error with matching quote\n", 2);
    else if (error_type == ERR_NEWCMD)
		ft_putstr_fd("command not found: ", 2);
    else if (error_type == ERR_PIPE)
		ft_putstr_fd("error creating pipe\n", 2);
    else if (error_type == ERR_PIPEND)
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
	else if (error_type == ERR_MEM)
		ft_putstr_fd("no memory left on device\n", 2);
	else if (error_type == ERR_ISDIR)
		ft_putstr_fd("Is a directory: ", 2);
    ft_putstr_fd(limit, 2);
    g_env.exit_status = status;
}

void error_cd(char **s[2])
{
    DIR *dir;
	
	dir = NULL;
    if ((!s[0][1] && s[1][0] && !s[1][0][0]) || (!s[0][1] && s[1][0] && (g_env.exit_status = chdir(s[1][0]) == -1)))
    {
        g_env.exit_status = 1;
        ft_putstr_fd("HOME not established!\n", 2);
        return;
    }
    if (s[0][1])
    {
        dir = opendir(s[0][1]);
        if (!dir && access(s[0][1], F_OK) == -1)
            m_error(ERR_NEWDIR, s[0][1], 1);
        else if (!dir)
            m_error(ERR_NOTDIR, s[0][1], 1);
        else
		{
            g_env.exit_status = chdir(s[0][1]) == -1;
            closedir(dir);
		}
    }
}

int	error_unset(char *av)
{
	write(2, "bash: unset: '", 14);
	write(2, av, ft_strlen(av));
	write(2, "' : not a valid identifier\n", 27);
	return (1);
}

void	m_perror(const char *msg, const char *detail, int exit_code)
{
	if (exit_code == ERR_DUP)
	{
		write(STDERR_FILENO, msg, strlen(msg));
	}
	if (detail)
	{
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, detail, strlen(detail));
	}
	write(STDERR_FILENO, "\n", 1);

	if (exit_code != 0)
		exit(exit_code);
}

void	ft_free(void *content)
{
	t_exec *node;
    
    node = content;
	free_form(&node->args);
	free(node->path);
	if (node->in != STDIN_FILENO)
		close(node->in);
	if (node->out != STDOUT_FILENO)
		close(node->out);
	free(node);
}
