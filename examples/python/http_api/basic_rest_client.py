"""
Basic REST API client example using requests.
"""

import sys
import requests
import functools
import json.decoder
from pprint import pformat


def main():
    default_url = 'http://127.0.0.1:8000'
    sys_url = sys.argv[1] if (len(sys.argv) >= 2) else None
    url = sys_url or default_url

    wait_enter = functools.partial(input, "Hit Enter ")

    def pprint_response(resp):
        try:
            print(pformat(resp.json()), "\n")
        except json.decoder.JSONDecodeError as e:
            print(e)

    print("Get current items (GET basic):")
    wait_enter()
    resp = requests.get(f"{url}/items/")
    pprint_response(resp)

    print("Get current items with filter (GET with query):")
    wait_enter()
    query = {'first': 1, 'limit': 3}
    resp = requests.get(f"{url}/items/", params=query)
    pprint_response(resp)

    print("Create new item (POST):")
    wait_enter()
    new_item = {
        'name': "monitor",
        'description': "LED 4K monitor",
        'price': 1500,
    }
    resp = requests.post(f"{url}/items/", json=new_item)
    pprint_response(resp)

    print("Get created item (GET with path param):")
    wait_enter()
    resp = requests.get(f"{url}/items/monitor")
    pprint_response(resp)

    print("Get current items again:")
    wait_enter()
    resp = requests.get(f"{url}/items/")
    pprint_response(resp)

    print("Destroy created item (DELETE with path param):")
    wait_enter()
    resp = requests.delete(f"{url}/items/monitor")
    pprint_response(resp)

    print("Try to get created item again:")
    wait_enter()
    resp = requests.get(f"{url}/items/monitor")
    pprint_response(resp)

if __name__ == "__main__":
    main()
