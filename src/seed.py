import sqlite3

DB_FILE="../attendance-checker.db"



conn = sqlite3.connect(DB_FILE)

query = """
CREATE TABLE users (
    tmrw_username TEXT PRIMARY KEY,
    telegram_username TEXT
);
"""
conn.execute(query)
