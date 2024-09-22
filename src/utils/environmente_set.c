/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environmente_set.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 19:24:57 by yasmine           #+#    #+#             */
/*   Updated: 2024/09/04 08:50:27 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char **update_or_add_env_var(char *variable, char *value)
{
    int i;
    int len;
    char *new_value;
    char    *tmp;

    i = 0;
    len = ft_strlen(variable);
    while(g_env.env[i])
    {
        if (ft_strncmp(g_env.env[i], variable, len - 1) == 0
            && g_env.env[i][len] == '=')
            break;
        i++;
    }
    free_str_array(g_env.sorted);
    if (g_env.env[i] == NULL)
        append_to_env(variable, value, i);
    else
    {
        free(g_env.env[i]);
        tmp = ft_strjoin(variable, "=");
		new_value = ft_strjoin(tmp, value);
		free(tmp);
		g_env.env[i] = new_value;
    }
    sort_array();
    return (g_env.env);
}