/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 11:46:39 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:50:11 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_quote_end(const char *str, int start, char quote_char)
{
	int	i;

	i = start + 1;
	while (str[i] && str[i] != quote_char)
		i++;
	return (i);
}

void	skip_quoted_section(const char *str, int *i)
{
	char	quote_char;

	quote_char = str[*i];
	*i = find_quote_end(str, *i, quote_char);
	if (str[*i] == quote_char)
		(*i)++;
}

void	skip_unquoted_section(const char *str, int *i)
{
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
}

int	count_segments(const char *str)
{
	int	i;
	int	seg_count;

	if (!str)
		return (0);
	i = 0;
	seg_count = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			skip_quoted_section(str, &i);
		else
			skip_unquoted_section(str, &i);
		seg_count++;
	}
	return (seg_count);
}
