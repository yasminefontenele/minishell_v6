/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_quote.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 08:59:10 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/09 22:26:21 by yfontene         ###   ########.fr       */
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

char *process_quotes(char *str)
{
    int     i;
    char    *content;
    char    *tmp;

    i = 0;
    content = ft_strdup("");
    if (str[i])
    {
        if (str[i] != '$')
            content = ft_stringjoin(content, str[i]);
        else
        {
            tmp = set_dollar(str, i);
            content = ft_stringjoin(content, tmp[i]);
            i += len_dollar(str, i) - 1;
        }
        i++;
    }
    return (content);
}

/*
Processes content within double or single quotes,
expanding variables as necessary.
*/
char *quotes_expand(char *content, int i)
{
    int nbr_of_dollars;
    char *tmp;

    tmp = ft_substr(content, i + 1, ft_strlen(content) - i - 2);
    nbr_of_dollars = dollar_presence(tmp);
    if (content[i] == '\"' && nbr_of_dollars != 0)
        tmp = process_quotes(tmp);
    return (tmp);
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
        if(i != 0)
            j++;
        if (tokens.tokens[i][j] == '\'' || tokens.tokens[i][j] == '\"')
        {
            tmp_token = tokens.tokens[i];
            tokens.tokens[i] = quotes_expand(tokens.tokens[i], i);
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
        i++;
    }
    //execution(tokens);   ********** TO DO **********
}