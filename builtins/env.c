/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scarlos- <scarlos-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:07:07 by scarlos-          #+#    #+#             */
/*   Updated: 2025/06/04 11:39:06 by scarlos-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_env_copy(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		free(env[i++]);
	free(env);
}

char	**copy_envp(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc((i + 1) * sizeof(char *));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
			return (free_env_copy(new_envp), NULL);
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static void	print_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		printf("%s\n", env[i++]);
}

void	ft_env(char **args, t_shell *shell)
{
	char	**copy;
	int		count;

	if (args && args[1])
	{
		print_error_command("env", "No such file or directory", 127, shell);
		return ;
	}
	count = 0;
	while (shell->envp[count])
		count++;
	copy = copy_envp(shell->envp);
	if (!copy)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	print_env(copy);
	free_env_copy(copy);
	shell->exit_status = 0;
}

void	update_env_var(char *name, char *value, t_shell *shell)
{
	char	*temp;
	char	*new_entry;
	int		index;

	temp = ft_strjoin(name, "=");
	new_entry = ft_strjoin(temp, value);
	free(temp);
	if (!shell->envp)
	{
		shell->envp = malloc(sizeof(char *) * 2);
		if (!shell->envp)
		{
			free(new_entry);
			return ;
		}
		shell->envp[0] = ft_strdup(new_entry);
		shell->envp[1] = NULL;
		free(new_entry);
		return ;
	}
	index = find_env_var_index(shell, name);
	shell->envp = copy_envp_with_update(shell, new_entry, index);
	free(new_entry);
}
