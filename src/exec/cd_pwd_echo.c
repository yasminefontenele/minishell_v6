/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd_echo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 15:15:09 by emencova          #+#    #+#             */
/*   Updated: 2024/09/25 08:33:03 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
/*
int	m_cd(t_shell *shell)
{
	char	**str[2];
	char	*home_dir;
	char	*current_dir;

	g_env.exit_status = 0;
	str[0] = ((t_exec *)shell->cmds->content)->args;
	home_dir = get_env("HOME", shell->keys, 4);
	if (!home_dir)
		home_dir = ft_strdup(" ");
	str[1] = extend_form(NULL, home_dir);
	free(home_dir);
	current_dir = getcwd(NULL, 0);
	str[1] = extend_form(str[1], current_dir);
	free(current_dir);
	error_cd(str);
	if (!g_env.exit_status)
		shell->keys = set_env("OLDPWD", str[1][1], shell->keys, 6);
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		current_dir = ft_strdup(" ");
	str[1] = extend_form(str[1], current_dir);
	free(current_dir);
	shell->keys = set_env("PWD", str[1][2], shell->keys, 3);
	free_form(&str[1]);
	return (g_env.exit_status);
}
//ULTIMA VESAO FUNCIONAL
int m_cd(t_shell *shell) {
    char **str;
    char *target_dir;
    char *current_dir;

    g_env.exit_status = 0;
    str = ((t_exec *)shell->cmds->content)->args;

    if (!str[1] || ft_strcmp(str[1], "") == 0)
	{
        target_dir = get_env("HOME", shell->keys, 4);
        if (!target_dir)
            target_dir = "/";
    } 
	else 
        target_dir = str[1];
    if (chdir(target_dir) == -1)
	{
        perror("cd");
        g_env.exit_status = 1;
    }
	else
	{
        current_dir = getcwd(NULL, 0);
        if (current_dir)
		{
            shell->keys = set_env("PWD", current_dir, shell->keys, 3);
            free(current_dir);
        }
    }

    return (g_env.exit_status);
}*/

int m_cd(t_shell *shell)
{
    char **str;
    char *target_dir;
    char *current_dir;

    g_env.exit_status = 0;
    str = ((t_exec *)shell->cmds->content)->args;

    // If "cd" has no arguments, go to HOME
    if (!str[1] || ft_strcmp(str[1], "") == 0)
	{
        target_dir = get_env("HOME", shell->keys, 4);
        if (!target_dir)
		{
            fprintf(stderr, "minishell: cd: HOME not set\n");
            g_env.exit_status = 1;
            return g_env.exit_status;
        }
    } 
    // Case "cd -" to go to the previous directory (OLDPWD)
    else if (ft_strcmp(str[1], "-") == 0)
	{
        target_dir = get_env("OLDPWD", shell->keys, 6);
        if (!target_dir)
		{
            fprintf(stderr, "minishell: cd: OLDPWD not set\n");
            g_env.exit_status = 1;
            return g_env.exit_status;
        }
        printf("%s\n", target_dir);// Show the path of the previous directory
	}
    else
        target_dir = str[1]; // Directory passed as argument

    // checks if the directory exists and is accessible
    if (access(target_dir, R_OK | X_OK) == -1)
	{
        perror("cd");
        g_env.exit_status = 1;
        return g_env.exit_status;
    }

    // Saves current directory to OLDPWD before switching
    current_dir = getcwd(NULL, 0);
    shell->keys = set_env("OLDPWD", current_dir, shell->keys, 6);
    free(current_dir);

    // Change to the destination directory
    if (chdir(target_dir) == -1)
	{
        perror("cd");
        g_env.exit_status = 1;
        return g_env.exit_status;
    }

    // Update PWD with new directory
    current_dir = getcwd(NULL, 0);
    if (current_dir)
	{
        shell->keys = set_env("PWD", current_dir, shell->keys, 3);
        free(current_dir);
    }
	else
	{
        perror("cd");
        g_env.exit_status = 1;
    }

    return g_env.exit_status;
}

int	m_pwd(void)
{
	char *buffer;

    buffer = getcwd(NULL, 1024);  // Altere 0 para um valor padrão, como 1024
    if (!buffer)
    {
        perror("pwd");
        return (1);
    }
    ft_putendl_fd(buffer, 1);
    free(buffer);
    return (0);
}
/*
int	m_echo(char **args, int ac)
{
	int i;
	int newline;

	
	i = 1;
	newline = 1;
	if (!args || !args[0])
		return (1);
	if (ac > 1 && args[1] && !ft_strcmp(args[1], "-n"))
	{
		newline = 0;
		i = 2;
	}
	while (i < ac && args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (i < ac - 1)
			write(STDOUT_FILENO, " ", 1); 
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}



int	m_echo(char **args)
{
	char	**path;
	int		i;

	path = args;
	i = 1;
	if (*args && args[0][0] == '-')
		while (args[0][i] == 'n')
			i++;
	if (*args && (ft_strncmp(*args, "-n", 2) == 0) && !args[0][i])
		path++;
	while (*path)
	{
		ft_putstr_fd(*path, STDOUT_FILENO);
		path++;
		if (*path)
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (!(*args && ft_strncmp(*args, "-n", 2) == 0 && !args[0][i]))
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
*/

/*int m_echo(char **args)
{
    char **path;
    int i;

    path = args;
    i = 1;
    if (*args && args[0][0] == '-')
    {
        while (args[0][i] == 'n')
            i++;
    }
    if (*args && (ft_strncmp(*args, "-n", 2) == 0) && !args[0][i])
        path++;
    if (*path)
        path++;
    while (*path)
    {
        ft_putstr_fd(*path, STDOUT_FILENO);
        path++;
        if (*path)
            ft_putchar_fd(' ', STDOUT_FILENO);
    }
    if (!(*args && ft_strncmp(*args, "-n", 2) == 0 && !args[0][i]))
        ft_putchar_fd('\n', STDOUT_FILENO);
    return (0);
}*/

//function changed to not print -n and not skip line
/*int m_echo(char **args)
{
    int i;
    int no_newline;

    i = 1;
	no_newline = 0;
    if (args[i] && strcmp(args[i], "-n") == 0)
	{
        no_newline = 1;
        i++;
    }
    while (args[i])
	{
        ft_putstr_fd(args[i], STDOUT_FILENO);
        i++;
        if (args[i])
            ft_putchar_fd(' ', STDOUT_FILENO);
    }
    if (!no_newline) {
        ft_putchar_fd('\n', STDOUT_FILENO);
    }
    return 0;
}*/

int m_echo(char **args, t_shell *shell)
{
    int i;
    int no_newline;
    char *processed_value;
    
    i = 1;
    no_newline = 0;
    if (args[i] && strcmp(args[i], "-n") == 0)
    {
        no_newline = 1;
        i++;
    }
    while (args[i])
    {
        processed_value = process_quotes(args[i], shell);
        ft_putstr_fd(processed_value, STDOUT_FILENO);
        free(processed_value);
        i++;
        if (args[i])
            ft_putchar_fd(' ', STDOUT_FILENO);
    }
    if (!no_newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
    return 0;
}

