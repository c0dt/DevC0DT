import unreal

rtg_file_name = f'/Game/Lucy/DEMO/Mannequin/Character/Rigs/RTG_Lucy'

source_ik_rig_filename = '/Game/Lucy/DEMO/Mannequin/Character/Rigs/IK_Lucy'
target_ik_rig_filename = '/Game/Characters/Mannequins/Rigs/IK_Mannequin'

source_skel_mesh_filename = '/Game/Lucy/DEMO/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'
target_skel_mesh_filename = '/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'

retarget_asset = unreal.load_asset(name = rtg_file_name)

rtg_controller = unreal.IKRetargeterController.get_controller(retarget_asset)

# Load the Source and Target IK Rigs.

source_ik_rig = unreal.load_asset(name = source_ik_rig_filename)

target_ik_rig = unreal.load_asset(name = target_ik_rig_filename)

# Assign the Source and Target IK Rigs.

rtg_controller.set_ik_rig(unreal.RetargetSourceOrTarget.SOURCE, source_ik_rig)

rtg_controller.set_ik_rig(unreal.RetargetSourceOrTarget.TARGET, target_ik_rig)

# Load the Skeletal Mesh.

source_skel_mesh = unreal.load_asset(name = source_skel_mesh_filename)

target_skel_mesh = unreal.load_asset(name = target_skel_mesh_filename)

# Assign the Source and Target Skeletal Meshes.

rtg_controller.set_preview_mesh(unreal.RetargetSourceOrTarget.SOURCE, source_skel_mesh)

rtg_controller.set_preview_mesh(unreal.RetargetSourceOrTarget.TARGET, target_skel_mesh)

# Use the asset subsystem to get asset data.

asset_subsystem = unreal.get_editor_subsystem(unreal.EditorAssetSubsystem)

# Get a list of asset data, which you can use for Anim Sequences, Anim Blueprints, Pose Assets, and more.

assets_to_retarget = [

        asset_subsystem.find_asset_data("/Game/Lucy/Animations/Battle/Lucy_Fight_Idle_Pose"),

        asset_subsystem.find_asset_data("/Game/Lucy/Animations/Battle/Lucy_Idle")
]

source_mesh = None # will use mesh from source ik rig

target_mesh = None # will use mesh from target ik rig

batch_op = unreal.IKRetargetBatchOperation.duplicate_and_retarget(

                                                assets_to_retarget,

                                                source_mesh,

                                                target_mesh,

                                                retarget_asset,

                                                search = "Game/Lucy",

                                                replace = "Game/LucyRetargeted",

                                                prefix = "",

                                                suffix = "",

                                                remap_referenced_assets = True)

print(batch_op)