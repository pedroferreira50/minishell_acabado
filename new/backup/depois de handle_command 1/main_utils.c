/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pviegas- <pviegas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 12:25:17 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 22:41:27 by pviegas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	finalize_shell(t_shell *shell)
{
	free_args(shell->envp, NULL);
	free_all_vars(&shell->vars);
	clear_history();
}

t_shell	*get_shell(void)
{
	static t_shell	shell;

	return (&shell);
}
