#pragma once
const int NUM_FRAGMENT_SHADERS = 18;
const char FRAGMENT_SHADERS[NUM_FRAGMENT_SHADERS][32] = {
	"fs_depth_s",
	"fs_text",
	"fs_particle",
	"fs_postprocess",
	"fs_blit",
	"fs_depth_masked_s",
	"fs_leaves_strand",
	"fs_aura",
	"fs_color_masked",
	"fs_dfsa",
	"fs_dfsa_color",
	"fs_dfsa_color_masked",
	"fs_dfsa_variation",
	"fs_flower_test",
	"fs_seed_test",
	"fs_selected",
	"fs_terrain",
	"fs_uibar",
};
