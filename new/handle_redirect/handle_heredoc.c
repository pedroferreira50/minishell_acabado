/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 03:47:00 by pviegas-          #+#    #+#             */
/*   Updated: 2025/06/05 04:08:07 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_mixed_quotes_delim(char *delim, size_t len)
{
	char	*clean_delim;
	size_t	j;
	size_t	i;

	clean_delim = malloc(len + 1);
	j = 0;
	i = 0;
	if (clean_delim)
	{
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
	}
	return (clean_delim);
}

static void	set_quoted_delim(t_command_data *data, char *delim, size_t len)
{
	data->heredoc_delim = ft_strndup(delim + 1, len - 2);
	data->heredoc_quoted = 1;
}

static void	set_mixed_quotes_delim(t_command_data *data, char *delim,
		size_t len)
{
	char	*clean_delim;

	clean_delim = process_mixed_quotes_delim(delim, len);
	if (clean_delim)
	{
		data->heredoc_delim = clean_delim;
		data->heredoc_quoted = 1;
	}
	else
	{
		data->heredoc_delim = ft_strdup(delim);
		data->heredoc_quoted = 0;
	}
}

static void	set_heredoc_delim(t_command_data *data, char *delim)
{
	size_t	len;

	len = ft_strlen(delim);
	if (len >= 2 && (delim[0] == '"' || delim[0] == '\'')
		&& delim[len - 1] == delim[0])
		set_quoted_delim(data, delim, len);
	else if (ft_strchr(delim, '\'') || ft_strchr(delim, '"'))
		set_mixed_quotes_delim(data, delim, len);
	else
	{
		data->heredoc_delim = ft_strdup(delim);
		data->heredoc_quoted = 0;
	}
}

void	handle_heredoc(char **args, t_command_data *data, t_indices *indices,
	t_shell *shell)
{
	char	*delim;

	if (args[indices->i + 1] != NULL)
	{
		if (data->heredoc_delim)
		{
			free(data->heredoc_delim);
			data->heredoc_delim = NULL;
		}
		delim = args[indices->i + 1];
		set_heredoc_delim(data, delim);
		if (data->heredoc_delim == NULL)
			shell->exit_status = 1;
		indices->i += 2;
	}
	else
	{
		print_error_simple("syntax error: no delimiter after '<<'",
			2, shell);
		indices->i++;
	}
}
