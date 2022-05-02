"""
Hello world FastAPI module

This module is to test if FastAPI can run correctly.
"""

from fastapi import FastAPI

app = FastAPI()


@app.get("/")
def root():
    return {"message": "Hello world!"}
