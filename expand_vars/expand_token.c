/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 04:46:00 by pviegas-          #+#    #+#             */
/*   Updated: 2025/06/04 04:49:40 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_mixed_quotes_token(char *token, t_shell *shell)
{
	char	*expanded;

	expanded = expand_mixed_quotes(token, shell);
	if (!expanded)
		expanded = ft_strdup("");
	return (expanded);
}

static char	*process_regular_token(char *token, char quote_type, t_shell *shell)
{
	char	*expanded;
	char	*tmp;

	if (quote_type != '\'')
		expanded = expand_variables(token, quote_type, shell);
	else
		expanded = ft_strdup(token);
	if (!expanded)
		expanded = ft_strdup(token);
	tmp = expanded;
	expanded = remove_quotes(tmp);
	free(tmp);
	if (!expanded)
		expanded = ft_strdup("");
	if (quote_type != '\'')
	{
		tmp = expanded;
		expanded = expand_tilde(tmp, shell);
		free(tmp);
	}
	return (expanded);
}

static char	*expand_single_token(char *token, char quote_type, t_shell *shell,
		int is_heredoc)
{
	if (is_heredoc)
		return (ft_strdup(token));
	if (has_mixed_quotes(token))
		return (process_mixed_quotes_token(token, shell));
	else
		return (process_regular_token(token, quote_type, shell));
}

static int	is_heredoc_delimiter(char **tokens, int index)
{
	if (index > 0 && ft_strcmp(tokens[index - 1], "<<") == 0)
		return (1);
	return (0);
}

char	**expand_tokens(char **tokens, char *quote_types, t_shell *shell)
{
	char		**expanded;
	t_indices	idx;

	idx.i = 0;
	if (!tokens || !tokens[0])
		return (NULL);
	expanded = malloc(sizeof(char *) * (count_args(tokens) + 1));
	if (!expanded)
		return (NULL);
	while (tokens[idx.i])
	{
		expanded[idx.i] = expand_single_token(tokens[idx.i],
				quote_types[idx.i], shell,
				is_heredoc_delimiter(tokens, idx.i));
		idx.i++;
	}
	expanded[idx.i] = NULL;
	return (expanded);
}
