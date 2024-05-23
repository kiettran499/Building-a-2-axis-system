# main.py
from fastapi import FastAPI
from pydantic import BaseModel
from typing import Optional, List
import socketio

app = FastAPI()
#Creates an asynchronous Socket.IO server using the ASGI mode.
sio = socketio.AsyncServer(async_mode='asgi')

# Attach the Socket.IO server to the FastAPI app
app_sio = socketio.ASGIApp(sio, other_asgi_app=app)

 
# Define a Pydantic model for the item
class Message(BaseModel):
    data1: Optional[int] = None
    data2: Optional[int] = None
    data3: Optional[str] = None
    # price: float
    # tax: Optional[float] = None

# create a list of database
# db: List[Item] = [
#     Item(
#         name="Kiet",
#         description="No",
#         price="0.5",
#         tax="0.1"
#     )
# ]   
 
@app.get("/")
async def read_root():
    return {"message": "Hello World"}

@app.get("/items/{item_id}")
async def read_item(item_id: int, q: str = None):
    return {"item_id": item_id, "q": q}

#get items from db
# @app.get("/items")
# async def register_item():
#     return db

# @app.post("/items/")
# def create_item(item: Item):
#     # db.append(item) #add a new item to items tree
#     return {"item": item}

# Defines a POST endpoint /send-message/ that accepts a JSON payload with a message and an optional room. 
# It uses sio.emit to send the message to all clients or to a specific room.


@app.post("/send-message/")
async def send_message(message: Message):
    await sio.emit('message', {'data1': message.data1, 'data2': message.data2, 'data3': message.data3})
    return {"status": "message sent"}


# Handles basic Socket.IO events for client connections, disconnections, and incoming messages.
# Define Socket.IO events
@sio.event
async def connect(sid, environ):
    print(f"Client connected: {sid}")

@sio.event
async def disconnect(sid):
    print(f"Client disconnected: {sid}")

# @sio.event
# async def message(sid, data):
#     print(f"Message from {sid}: {data}")
#     await sio.emit('response', {'response': 'my response'})

@sio.event
async def message(sid, data):
    print(f"Message from {sid}: {data}")


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app_sio, host="127.0.0.1", port=8000)
