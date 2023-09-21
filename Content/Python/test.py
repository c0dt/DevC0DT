# import unreal
# obj = unreal.MediaPlayer()
# # Modifying a property directly can have different results
# # than changing settings in the Editor UI.
# # Generally you'll want to avoid setting these values directly, like this:
# obj.play_on_open = True
# # This way of accessing the property will have exactly the same
# # result as changing the setting in the Editor UI:
# obj.set_editor_property("play_on_open", True)
# # Both ways of reading the value are equivalent.
# # When a class exposes a property in both ways, you can use either:
# play_value = obj.play_on_open
# play_value = obj.get_editor_property("play_on_open")
# print(obj)


print("Hello WOrld")