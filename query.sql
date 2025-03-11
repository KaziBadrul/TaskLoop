-- -- Table for users (already exists)
-- CREATE TABLE IF NOT EXISTS users (
--     id INTEGER PRIMARY KEY AUTOINCREMENT,
--     username TEXT UNIQUE NOT NULL,
--     email TEXT UNIQUE NOT NULL,
--     password TEXT NOT NULL
-- );

-- -- Table for groups
-- CREATE TABLE IF NOT EXISTS groups (
--     id INTEGER PRIMARY KEY AUTOINCREMENT,
--     name TEXT NOT NULL,
--     owner_id INTEGER NOT NULL,
--     FOREIGN KEY (owner_id) REFERENCES users(id)
-- );

-- -- Table for group members (user's relationship with groups)
-- CREATE TABLE IF NOT EXISTS group_members (
--     group_id INTEGER NOT NULL,
--     user_id INTEGER NOT NULL,
--     FOREIGN KEY (group_id) REFERENCES groups(id),
--     FOREIGN KEY (user_id) REFERENCES users(id),
--     PRIMARY KEY (group_id, user_id)
-- );

-- -- Table for tasks
-- CREATE TABLE IF NOT EXISTS tasks (
--     id INTEGER PRIMARY KEY AUTOINCREMENT,
--     name TEXT NOT NULL,
--     description TEXT NOT NULL,
--     due_date TEXT NOT NULL,
--     priority INTEGER NOT NULL, -- Enum mapped to integer
--     status INTEGER NOT NULL,  -- Enum mapped to integer
--     group_id INTEGER NOT NULL,
--     FOREIGN KEY (group_id) REFERENCES groups(id)
-- );

-- SELECT * FROM groups;

CREATE TABLE tasks (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    description TEXT,
    due_date INTEGER NOT NULL,  
    priority INTEGER NOT NULL, -- 1 = Low, 2 = Medium, 3 = High
    status INTEGER NOT NULL DEFAULT 0, -- 0 = Pending, 1 = In Progress, 2 = Completed
    group_id INTEGER NOT NULL,
    creator_id INTEGER NOT NULL, -- Links to users table
    creator_name TEXT NOT NULL, -- Store the creator's name directly
    FOREIGN KEY (group_id) REFERENCES groups(id) ON DELETE CASCADE,
    FOREIGN KEY (creator_id) REFERENCES users(userId) ON DELETE CASCADE
);

-- CREATE TABLE tasks_new (
--     id INTEGER PRIMARY KEY AUTOINCREMENT,
--     name TEXT NOT NULL,
--     description TEXT,
--     due_date INTEGER NOT NULL,  -- Changed from TEXT to INTEGER
--     priority INTEGER NOT NULL,
--     status INTEGER NOT NULL DEFAULT 0,
--     group_id INTEGER NOT NULL,
--     creator_id INTEGER NOT NULL,
--     creator_name TEXT NOT NULL,
--     FOREIGN KEY (group_id) REFERENCES groups(id) ON DELETE CASCADE,
--     FOREIGN KEY (creator_id) REFERENCES users(userId) ON DELETE CASCADE
-- );
