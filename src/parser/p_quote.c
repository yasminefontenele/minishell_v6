/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_quote.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 08:59:10 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/23 09:55:27 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int dollar_presence(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == '$')
            return (1);
        i++;
    }
    return (0);
}

char *quotes_expand(char *content, int i)
{
    int nbr_of_dollars;
    char *tmp;

    if (content[i] == '\"')
    {
        tmp = ft_substr(content, i + 1, ft_strlen(content) - i - 2);
        nbr_of_dollars = dollar_presence(tmp);
        if (nbr_of_dollars != 0)
            tmp = dollar_config(tmp, 0);
        return tmp;
    }
    if (content[i] == '\'')
        return ft_substr(content, i + 1, ft_strlen(content) - i - 2);
    return ft_strdup(content);
}

char *process_quotes(char *str)
{
    int i;
    
    i = 0;
    if (str[i] == '\'')
    {
        i++;
        while (str[i] && str[i] != '\'')
            i++;
        return ft_substr(str, 1, i - 1);
    }
    if (str[i] == '\"')
    {
        return quotes_expand(str, i);
    }
    return ft_strdup(str);
}

/*
Iterates over the tokens (parts of the command separated by spaces or
other delimiters) and applies procesc_quotes to each token that contains quotes
*/
t_tokens process_quotes_tokens(t_tokens tokens)
{
    int i;
    int j;
    char *tmp_token;

    i = 0;
    while (tokens.tokens[i])
    {
        j = 0;
        // Verifica se o token tem aspas no começo
        if (tokens.tokens[i][j] == '\'' || tokens.tokens[i][j] == '\"')
        {
            tmp_token = tokens.tokens[i];
            tokens.tokens[i] = quotes_expand(tokens.tokens[i], j);
            free(tmp_token);
        }
        i++;
    }
    return (tokens);
}


/*
ensures that each part of the command correctly processes variables and
quotes before execution.
*/
void exec_process_quotes(t_tokens *tokens)
{
    int i;

    i = 0;
    while (i < tokens[0].pipe)
    {
        tokens[i] = process_quotes_tokens(tokens[i]);
        int j = 0;
        while (tokens[i].tokens[j])
        {
            j++;
        }
        i++;
    }
    //execution(tokens);   ********** TO DO **********
}
