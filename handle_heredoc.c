/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:19:14 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:19:17 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_quoted_delimiter(char *delim, size_t len)
{
	if (len >= 2 && (delim[0] == '"' || delim[0] == '\'')
		&& delim[len - 1] == delim[0])
	{
		return (ft_strndup(delim + 1, len - 2));
	}
	return (NULL);
}

char	*clean_mixed_quotes(char *delim, size_t len)
{
	char	*clean_delim;
	size_t	j;
	size_t	i;

	clean_delim = malloc(len + 1);
	if (!clean_delim)
		return (NULL);
	j = 0;
	i = 0;
	while (i < len)
	{
		if (delim[i] != '\'' && delim[i] != '"')
		{
			clean_delim[j] = delim[i];
			j++;
		}
		i++;
	}
	clean_delim[j] = '\0';
	return (clean_delim);
}

static void	assign_heredoc_data(t_command_data *data, char *delim, int quoted)
{
	data->heredoc_delim = delim;
	data->heredoc_quoted = quoted;
}

void	set_heredoc_delimiter(t_command_data *data, char *delim)
{
	size_t	len;
	char	*processed_delim;

	len = ft_strlen(delim);
	processed_delim = process_quoted_delimiter(delim, len);
	if (processed_delim)
		assign_heredoc_data(data, processed_delim, 1);
	else if (ft_strchr(delim, '\'') || ft_strchr(delim, '"'))
	{
		processed_delim = clean_mixed_quotes(delim, len);
		if (processed_delim)
			assign_heredoc_data(data, processed_delim, 1);
		else
			assign_heredoc_data(data, ft_strdup(delim), 0);
	}
	else
		assign_heredoc_data(data, ft_strdup(delim), 0);
}

int	validate_heredoc_args(char **args, t_indices *indices, t_shell *shell)
{
	if (args[indices->i + 1] == NULL)
	{
		print_error_simple("syntax error: no delimiter after '<<'", 2, shell);
		indices->i++;
		return (-1);
	}
	return (0);
}
