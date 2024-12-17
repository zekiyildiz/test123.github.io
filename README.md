CENG209 Dungeon Game
Overview

This is a text-based adventure game developed in C, designed for the CENG209 course. Players navigate through a dungeon, collect items, engage in battles with creatures, and retrieve the golden crown. Utilize strategic commands to manage your inventory, restore health, and overcome challenges within the dungeon's depths.


Gameplay
Objective

Your mission is to navigate through the dungeon, defeat creatures, and retrieve the golden crown. Return the crown to the starting point to claim your victory and complete the game.
Commands

Interact with the game using the following text-based commands:

    move <direction>: Move in a specified direction (up, down, left, right).

>> move right

look: Observe your current surroundings, including available exits and items.

>> look

pickup <item>: Collect an item from the current room and add it to your inventory.

>> pickup iron_sword

Note: Generic items like juice_machine cannot be picked up and must be used directly.

drop <item>: Remove an item from your inventory and leave it in the current room.

>> drop iron_sword

use <item>: Utilize an item for its intended purpose. For example, use the juice_machine to restore health.

>> use juice_machine

attack: Engage in combat with a creature present in the room.

>> attack

inventory: Display all items currently in your possession.

>> inventory

equip <item>: Equip an item from your inventory to enhance your abilities.

>> equip iron_sword

unequip <item>: Remove an equipped item from your character.

>> unequip iron_sword

rest: Take a moment to recover some health.

>> rest

status: View your current status, including level, experience, health, attack, defense, and gold.

>> status

map: Display a map of the rooms you have discovered so far.

>> map

save <filepath>: Save your current game state to a file.

>> save mygame.save

load <filepath>: Load a previously saved game state from a file.

>> load mygame.save

list: List all saved game files in the current directory.

>> list

help: Show a list of all available commands.

>> help

exit: Exit the game.

    >> exit

Game Elements

The player character has the following attributes:

    Health: Represents the player's current health points.
    Max Health: The maximum health points the player can have.
    Attack: Determines the player's attack power.
    Defense: Determines the player's defense capability.
    Gold: Currency used to perform certain actions, like using the juice machine.
    Experience (XP): Gained by defeating creatures, leading to level-ups.
    Level: Indicates the player's current level.
    Inventory: Holds items the player has collected.
    Equipped Items: Items currently equipped to enhance abilities.

Items

Items in the game are categorized into different types:

    Weapons: Increase the player's attack power.
    Armor: Increase the player's defense.
    Food: Can be consumed to restore health.
    Generic: Items that cannot be picked up and must be used directly (e.g., juice_machine).
    Quest Items: Special items required to complete certain objectives.

Rooms

Rooms are interconnected spaces within the dungeon. Each room has:

    Description: Text describing the room.
    Exits: Possible directions to move (up, down, left, right).
    Items: Items available in the room.
    Creatures: Enemies or creatures present in the room.


Creatures

Creatures are enemies that the player can engage in combat with. Defeating creatures rewards the player with experience points (XP) and gold.


Code Structure

The game is organized into several source (.c) and header (.h) files, each handling different aspects of the game’s functionality. This modular approach ensures code readability, maintainability, and ease of debugging.
Source Files

    main.c: Contains the main function, game loop, and command processing logic.
    game.c: Manages overall game state, initialization, and core game functions.
    player.c: Handles player-related operations, including inventory management and status updates.
    room.c: Manages room creation, item placement, and creature interactions within rooms.
    item.c: Deals with item creation, usage, and inventory interactions.
    creature.c: Handles creature behaviors, combat mechanics, and interactions with the player.
 
