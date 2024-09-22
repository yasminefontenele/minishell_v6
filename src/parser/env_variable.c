/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 08:48:49 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/22 08:59:47 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*char    *find_env_value(char *str, int i)
{
    int     j;
    int     name_len;
    char    *env_value;

    j = 0;
    env_value = NULL;
    if (i != 0)
        i = 1;
    name_len = ft_strlen(str + 1);
    while (g_env.sorted[j])
    {
        if (ft_strncmp(str + 1 + i, g_env.sorted[j], i - 1) == 0)
        {
            env_value = ft_substr(g_env.sorted[j], name_len, ft_strlen(g_env.sorted[j]) - name_len + 2);
            free(str);
            str = NULL;
            return (env_value);
        }
        j++;
    }
    free(str);
    str = NULL;
    return (ft_strdup(""));
}*/

char *find_env_value(char *str, int i)
{
    int     j;
    int     name_len;
    char    *env_value;

    j = 0;
    env_value = NULL;
    if (str[i] == '$')
        i++;
    name_len = 0;
    while (str[i + name_len] && (ft_isalnum(str[i + name_len]) || str[i + name_len] == '_'))
        name_len++;
    while (g_env.sorted[j])
    {
        if (ft_strncmp(str + i, g_env.sorted[j], name_len) == 0 && g_env.sorted[j][name_len] == '=')
        {
            env_value = ft_strdup(ft_strchr(g_env.sorted[j], '=') + 1);
            return (env_value);
        }
        j++;
    }
    return (ft_strdup(""));
}


