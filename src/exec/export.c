/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eliskam <eliskam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 08:08:44 by emencova          #+#    #+#             */
/*   Updated: 2024/09/26 08:43:47 by eliskam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../minishell.h"
#include "execute.h"


int is_valid_env_var(const char *var_name)
{
    int i;

    i = 0;
    if (!var_name || ft_strlen(var_name) == 0)
        return (0);

    if (!isalpha(var_name[0]) && var_name[0] != '_')
        return (0);

    while (var_name[i] != '\0')
    {
        if (!ft_isalnum(var_name[i]) && var_name[i] != '_')
            return (0);
        i++;
    }

    return (1);
}


void split_var_value(char *arg, char **var, char **value)
{
    char *eq_pos = ft_strchr(arg, '=');
    if (eq_pos)
    {
        *var = ft_strndup(arg, eq_pos - arg);
        *value = ft_strdup(eq_pos + 1);
    }
    else
    {
        *var = ft_strdup(arg);
        *value = NULL;
    }
}
/*
int find_key_idx(char **keys,char *key)
{
    int i;
    int key_len;
    
    key_len = ft_strchr(key, '=') - key;
    i = 0;
    while (keys[i])
    {
        if (!ft_strncmp(keys[i], key, key_len) && keys[i][key_len] == '=')
            return (i);
        i++;
    }

    return (-1);
}*/

void print_all_variables(char **keys)
{
    int i;
    
    i = 0;
    while (keys[i])
    {
        printf("declare -x %s\n", keys[i]);
        i++;
    }
}

int m_export(t_shell *shell)
{
    int i;
    char **av;
    char *var_name;
    char *value;
    char *equals_sign; 
    
    var_name = NULL,
    value = NULL;
    av = ((t_exec *)shell->cmds->content)->args;
    i = 1;
    if (!av[1])
    {
        print_all_variables(shell->keys);
        return (0);
    }
    while (av[i])
    {
        equals_sign = ft_strchr(av[i], '=');
        if (equals_sign != NULL)
        {
            split_var_value(av[i], &var_name, &value);
            if (is_valid_env_var(var_name))
            {
                int index = find_key_idx(shell->keys, var_name);
                if (index != -1)
                {
                    free(shell->keys[index]);
                    shell->keys[index] = strdup(av[i]);
                }
                else
                    shell->keys = extend_form(shell->keys, av[i]);
            }
            else
                write(STDERR_FILENO, "Not a valid identifier\n", 23);
            free(var_name);
            free(value);
        }
        else
        {
            if (is_valid_env_var(av[i]))
            {
                int index = find_key_idx(shell->keys, av[i]);
                if (index == -1)
                    shell->keys = extend_form(shell->keys, av[i]);
            }
            else
                write(STDERR_FILENO, "Not a valid identifier\n", 23);
        }
        i++;
    }
    return (1);
}