include $(PWD)/config.mk
FOLDER_NAME = src/gp

default: evolve_gp

gp_tree:
	@make -s -C tree

gp_cartesian:
	@make -s -C cartesian

evolve_gp: gp_tree gp_cartesian
