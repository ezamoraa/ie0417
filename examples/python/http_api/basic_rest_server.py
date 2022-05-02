"""
Basic REST API server example using FastAPI.
"""

from fastapi import FastAPI, Body
from random import randint
from typing import Optional
from pydantic import BaseModel

class Item(BaseModel):
    name: str
    description: Optional[str] = None
    price: int

app = FastAPI()

item_descs = [
    ("laptop", "Gaming Laptop PC"),
    ("cellphone", "Android Smartphone"),
    ("tablet", ""),
    ("workstation", "Gaming Workstation PC"),
    ("smartwatch", ""),
]

items = {
    name: Item(
        name=name,
        description=desc,
        price=randint(1000, 10000)
    )
    for name, desc in item_descs
}


@app.post("/items/")
def create_item(item: Item):
    """
    Create a new item and register it

    :param Item item: Item to register.
    """
    items[item.name] = item
    return item


@app.get("/items/")
def read_items(first: int = 0, limit: int = 10):
    """
    Get a list of the current items.

    :param int first: First list element to get (optional).
    :param int limit: Maximum number of elements to get (optional).
    """
    items_list = [item for item in items.values()]
    return items_list[first : first + limit]


@app.delete("/items/{item_name}")
def delete_item(item_name: str, status_code=204):
    """
    Unregister and delete item.

    :param str item_name: Name of the item to delete.
    :param int status_code: Default HTTP status code to return.
    """
    del items[item_name]


@app.get("/items/{item_name}")
def read_item(item_name: str):
    """
    Get specific item from name.

    :param str item_name: Name of the item to get.
    """
    return items[item_name]
