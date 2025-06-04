/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quote_segments.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 07:13:00 by pviegas-          #+#    #+#             */
/*   Updated: 2025/06/04 07:16:19 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	find_quote_end(const char *str, int start, char quote_char)
{
	int	i;

	i = start + 1;
	while (str[i] && str[i] != quote_char)
		i++;
	return (i);
}

static void	skip_quoted_section(const char *str, int *i)
{
	char	quote_char;

	quote_char = str[*i];
	*i = find_quote_end(str, *i, quote_char);
	if (str[*i] == quote_char)
		(*i)++;
}

static void	skip_unquoted_section(const char *str, int *i)
{
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
}

static int	count_segments(const char *str)
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

static void	fill_quoted_segment(const char *str, t_quote_segment *segment,
		int *i, int *count)
{
	int		start;
	char	quote_char;

	start = *i;
	quote_char = str[*i];
	*i = find_quote_end(str, *i, quote_char);
	if (str[*i] == quote_char)
		(*i)++;
	segment[*count].quote_type = quote_char;
	segment[*count].start = start;
	segment[*count].end = *i;
	segment[*count].content = ft_strndup(&str[start], *i - start);
	(*count)++;
}

static void	fill_unquoted_segment(const char *str, t_quote_segment *segment,
		int *i, int *count)
{
	int	start;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
		(*i)++;
	segment[*count].quote_type = '\0';
	segment[*count].start = start;
	segment[*count].end = *i;
	segment[*count].content = ft_strndup(&str[start], *i - start);
	(*count)++;
}

static void	fill_segments(const char *str, t_quote_segment *segments,
		int *count)
{
	int	i;

	i = 0;
	*count = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			fill_quoted_segment(str, segments, &i, count);
		else
			fill_unquoted_segment(str, segments, &i, count);
	}
}

t_quote_segment	*parse_quote_segments(const char *str, int *count)
{
	t_quote_segment	*segments;
	int				seg_count;

	*count = 0;
	if (!str)
		return (NULL);
	seg_count = count_segments(str);
	segments = malloc(sizeof(t_quote_segment) * seg_count);
	if (!segments)
		return (NULL);
	fill_segments(str, segments, count);
	return (segments);
}