import unreal
from datetime import datetime
# source_skel_mesh_filename = '/Game/Lucy/DEMO/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'
# target_skel_mesh_filename = '/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'
# source_skel_mesh = unreal.load_asset(name = source_skel_mesh_filename)
# target_skel_mesh = unreal.load_asset(name = target_skel_mesh_filename)
def setupRetarger():
    rtg_file_name = f'/Game/Lucy/DEMO/Mannequin/Character/Rigs/RTG_Lucy'

    source_ik_rig_filename = '/Game/Lucy/DEMO/Mannequin/Character/Rigs/IK_Lucy'
    target_ik_rig_filename = '/Game/Characters/Mannequins/Rigs/IK_Mannequin'

    source_skel_mesh_filename = '/Game/Lucy/DEMO/Mannequin/Character/Mesh/SK_Mannequin_Female.SK_Mannequin_Female'
    target_skel_mesh_filename = '/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'

    retarget_asset = unreal.load_asset(name = rtg_file_name)

    # rtg_controller = unreal.IKRetargeterController.get_controller(retarget_asset)

    # # Load the Source and Target IK Rigs.

    # source_ik_rig = unreal.load_asset(name = source_ik_rig_filename)

    # target_ik_rig = unreal.load_asset(name = target_ik_rig_filename)

    # # Assign the Source and Target IK Rigs.

    # rtg_controller.set_ik_rig(unreal.RetargetSourceOrTarget.SOURCE, source_ik_rig)

    # rtg_controller.set_ik_rig(unreal.RetargetSourceOrTarget.TARGET, target_ik_rig)

    # # Load the Skeletal Mesh.

    # source_skel_mesh = unreal.load_asset(name = source_skel_mesh_filename)

    # target_skel_mesh = unreal.load_asset(name = target_skel_mesh_filename)

    # # Assign the Source and Target Skeletal Meshes.

    # rtg_controller.set_preview_mesh(unreal.RetargetSourceOrTarget.SOURCE, source_skel_mesh)

    # rtg_controller.set_preview_mesh(unreal.RetargetSourceOrTarget.TARGET, target_skel_mesh)

    return retarget_asset

#  return Array[AssetData]
def duplicate_and_retarget(retarget_asset, package_name):
    asset_subsystem = unreal.get_editor_subsystem(unreal.EditorAssetSubsystem)
    assets_to_retarget = [
        asset_subsystem.find_asset_data(package_name),
    ]
    print(assets_to_retarget)
    source_mesh = None # will use mesh from source ik rig
    target_mesh = None # will use mesh from target ik rig
    batch_result = unreal.IKRetargetBatchOperation.duplicate_and_retarget(
                                                    assets_to_retarget,
                                                    source_mesh,
                                                    target_mesh,
                                                    retarget_asset,
                                                    search = "",
                                                    replace = "",
                                                    prefix = "",
                                                    suffix = "",
                                                    remap_referenced_assets = True)
    return batch_result[0] if len(batch_result) == 1 else None

def main():
    # lenOfArgv = len(sys.argv)
    # if lenOfArgv != 3:
    #     print("Incorrect arguments. Usage: python main.py /path/to/your/input /path/to/your/output ")
    #     return
    
    source_folder_path = "/Game/Lucy"
    target_folder_path = "/Game/Lucy3"

    retargeter_asset = setupRetarger()

    filter = unreal.ARFilter(
        class_paths = [unreal.TopLevelAssetPath(package_name = '/Script/Engine', asset_name = 'AnimSequence')],
        recursive_paths = True,
        package_paths = [unreal.Name(source_folder_path)],
    )
    print(filter)
    # List all assets in the folder
    asset_data_list = unreal.AssetRegistryHelpers.get_asset_registry().get_assets(filter)
    asset_data_list_len = len(asset_data_list)
    for i in range(asset_data_list_len):
        asset_data = asset_data_list[i]
        source_package_path = unreal.StringLibrary.conv_name_to_string(asset_data.package_path)
        source_package_name = unreal.StringLibrary.conv_name_to_string(asset_data.package_name)
        target_package_path = source_package_path.replace(source_folder_path, target_folder_path, 1)
        target_package_name = source_package_name.replace(source_folder_path, target_folder_path, 1)
        try:
            retarget_result = duplicate_and_retarget(retargeter_asset, source_package_name)
        except:
            print("error")
        if retarget_result != None:
            unreal.EditorAssetLibrary.make_directory(target_package_path)
            unreal.EditorAssetLibrary.rename_asset(unreal.StringLibrary.conv_name_to_string(retarget_result.package_name),target_package_name)

        print(f'finished {i+1}/{asset_data_list_len}')
        if i>10:
            break

    print('All Done')


if __name__ == '__main__':
    main()