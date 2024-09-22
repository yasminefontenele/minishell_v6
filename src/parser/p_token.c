/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_token.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 18:11:10 by yfontene          #+#    #+#             */
/*   Updated: 2024/09/21 11:16:12 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

//searches for the end of a token that starts with $
int token_dollar_end(char *str, int i)
{
    if (str[i + 1] && str[i + 1] == '$')
        return (1 + 2);
    i++;
    while (str[i])
    {
        if (str[i] == '$' || str[i] == ' ' || str[i] == '\t'
			|| str[i] == '\'' || str[i] == '\"' || str[i] == '>'
			|| str[i] == '<' || str[i] == '\0')//
            return (i);
        else
            i++;
    }
    return (i);
}

int token_word_end(char *str, int i)
{
    while (str[i])
    {
        if (str[i] == '$' || str[i] == ' ' || str[i] == '\t'
            || str[i] == '>' || str[i] == '<' || str[i] == '\0')
            return (i);
        else if ((str[i] == '\'' || str[i] == '\"')
            && (count_backslash(str, i) % 2 == 0))
            return (i);
        else
            i++;
    }
    return (i);
}

int token_quotes_end(char *str, int i)
{
    int j;

    j = i + 1;
    while (str[i])
    {
        if (str[j] == str[i] && str[i] == '\'')
            return (j + 1);
        if (str[j] == str[i] && (count_backslash(str, j) % 2 == 0))
            return (j + 1);
        j++;
    }
    return (-1);//did not find a matching quote to close the token.
}

int token_redir_end(char *str, int i)
{
    int j;

    j = i;
    while (str[i])
    {
        if (str[j] == str[i])
            i++;
        else
            return (i);
    }
    return (i);
}

char *parse_next_token(char *line, int reset)
{
    static int current_pos = 0;
    char *token;
    int end;
    int initial_offset;

    token = NULL;
    initial_offset = 0;
    if (reset == 0)
        current_pos = 0;
    if (current_pos == 0)
        initial_offset = 0;
    while (line[current_pos])
    {
        current_pos = skip_space(line, current_pos);
        if (line[current_pos])
        {
            end = get_end(line, current_pos);
            if (end != -1)
                token = extract_substring(line, current_pos - initial_offset, end);
            current_pos = end;
            return (token);
        }
    }
    return (token);
}
