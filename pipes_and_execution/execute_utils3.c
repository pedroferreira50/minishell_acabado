/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 12:13:37 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 12:14:02 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	finalize_execution(t_exec_state *state, pid_t *pids,
	t_command_data *data, t_shell *shell)
{
	int	save_exit;

	save_exit = shell->exit_status;
	wait_commands(pids, data, shell);
	if (!get_shell()->is_save_to_execute)
		shell->exit_status = save_exit;
	get_shell()->is_save_to_execute = true;
	if (state->prev_pipe_read != -1)
	{
		close(state->prev_pipe_read);
		state->prev_pipe_read = -1;
	}
}
