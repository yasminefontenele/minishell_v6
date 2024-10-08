/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yfontene <yfontene@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:25:13 by emencova          #+#    #+#             */
/*   Updated: 2024/09/23 09:22:49 by yfontene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

char *read_here_document(char *input_buffer[2], size_t limit_length, char *end_marker, char *error_message)
{
    char *temp;

    while (g_env.exit_status != 130 && (input_buffer[0] == NULL
     || ft_strncmp(input_buffer[0], end_marker, limit_length) != 0 
     || ft_strlen(end_marker) != limit_length))
    {
        temp = input_buffer[1];
        input_buffer[1] = ft_strjoin(input_buffer[1], input_buffer[0]);
        free(temp);
        free(input_buffer[0]);
        input_buffer[0] = readline("> ");
        if (input_buffer[0] == NULL)
        {
            printf("%s (wanted `%s`)\n", error_message, end_marker);
            break;
        }
        temp = input_buffer[0];
        input_buffer[0] = ft_strjoin(input_buffer[0], "\n");
        free(temp);
        limit_length = ft_strlen(input_buffer[0]) - 1;
    }
    free(input_buffer[0]);
    return (input_buffer[1]);
}

int create_here_document_fd(char *input_buffer[2], char *delimiter[2])
{
    int fd[2];

    g_env.exit_status = 0;
    if (pipe(fd) == -1)
    {
        m_perror(PIPE_READ, NULL, 1);
        return (-1);
    }
    input_buffer[1] = read_here_document(input_buffer, 0, delimiter[0], delimiter[1]);
    write(fd[PIPE_WRITE], input_buffer[1], ft_strlen(input_buffer[1]));
    free(input_buffer[1]);
    close(fd[PIPE_WRITE]);

    if (g_env.exit_status == 130)
    {
        close(fd[PIPE_READ]);
        return (-1);
    }
    return (fd[PIPE_READ]);
}

