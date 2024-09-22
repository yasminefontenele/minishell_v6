/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 20:08:35 by yasmine           #+#    #+#             */
/*   Updated: 2024/09/07 10:41:41 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//This function is used to format the environment variable
char	*format_var(char *var, char *value)
{
    char    *new_var;
    int     i;
    int     j;

    i = -1;
    new_var = malloc(sizeof(char) * (ft_strlen(var) + ft_strlen(value) + 2));
    if(new_var == NULL)
        return (NULL);
    while (var[++i])
        new_var[i] = var[i];
    new_var[i] = '=';
    j = -1;
    while (value[++j])
        new_var[i++] = value[j];
    new_var[i] = '\0';
    return (new_var);
}

//This function is used to add a new variable to the environment
void new_var(char *var, char *value)
{
    int    i;
    char    size;
    char    **new_env;
    char    *new_value;
    
    i = -1;
    size = count(g_env.env);
    new_env = malloc(sizeof(char *) * (size + 2));
    while(g_env.env[++i])
        new_env[i] = g_env.env[i];
    new_value = format_var(var, value);
    new_env[i] = ft_strdup(new_value);
    new_env[i + 1] = NULL;
    free_str_array(g_env.env);//free the old environment
    free_str_array(g_env.sorted);//free the old environment
    g_env.env = new_env;//assign the new environment
    sort_array();
}

//This function is used to update the environment variable
void update_env(char *var, char *value)
{
    int i;
    int var_len;

    i = -1;
    var_len = ft_strlen(var);
    while (g_env.env[++i])
    {
        if (ft_strncmp(var, g_env.env[i], var_len) == 0)//if the variable already exists
        {
            env_update(var, value);
            return ;
        }
    }
    new_var(var, value);//if the variable does not exist
}