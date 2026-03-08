# Copyright (c) 2022-2025, The Isaac Lab Project Developers.
# All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause

import gymnasium as gym

from .my_quadruped_env_cfg import MyQuadrupedEnvCfg, MyQuadrupedEnvCfg_PLAY
# Pull in the PPO agent config for registration
from .agents import rsl_rl_ppo_cfg

##
# Register the environment!
##

gym.register(
    id="Isaac-MyQuadruped-v0",
    entry_point="isaaclab.envs:ManagerBasedRLEnv",
    kwargs={
        "env_cfg_entry_point": MyQuadrupedEnvCfg,
        # Link the PPO runner config to this environment entry
        "rsl_rl_cfg_entry_point": rsl_rl_ppo_cfg.AntPPORunnerCfg,
    },
    disable_env_checker=True,
)

gym.register(
    id="Isaac-MyQuadruped-Play-v0",
    entry_point="isaaclab.envs:ManagerBasedRLEnv",
    kwargs={
        "env_cfg_entry_point": MyQuadrupedEnvCfg_PLAY,
        # Link the PPO runner config to the play variant as well
        "rsl_rl_cfg_entry_point": rsl_rl_ppo_cfg.AntPPORunnerCfg,
    },
    disable_env_checker=True,
)