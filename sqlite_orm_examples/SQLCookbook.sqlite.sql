BEGIN TRANSACTION;
DROP TABLE IF EXISTS "Albums";
CREATE TABLE IF NOT EXISTS "Albums" (
	"id"	INTEGER NOT NULL,
	"artist_id"	INTEGER NOT NULL,
	"year"	INTEGER NOT NULL,
	"age"	INTEGER NOT NULL,
	FOREIGN KEY("artist_id") REFERENCES "Artists"("id"),
	PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "Artists";
CREATE TABLE IF NOT EXISTS "Artists" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"born"	TEXT NOT NULL DEFAULT (0),
	PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "Emp_bonus";
CREATE TABLE IF NOT EXISTS "Emp_bonus" (
	"id"	INTEGER NOT NULL,
	"empno"	INTEGER NOT NULL,
	"received"	TEXT NOT NULL,
	"type"	INTEGER NOT NULL,
	FOREIGN KEY("empno") REFERENCES "Emp"("empno"),
	PRIMARY KEY("id" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "Dept";
CREATE TABLE IF NOT EXISTS "Dept" (
	"deptno"	INTEGER NOT NULL,
	"deptname"	TEXT NOT NULL,
	"loc"	TEXT NOT NULL,
	"mgr"	INTEGER,
	"num_employees"	INTEGER NOT NULL DEFAULT (0),
	FOREIGN KEY("mgr") REFERENCES "Emp"("empno"),
	PRIMARY KEY("deptno" AUTOINCREMENT)
);
DROP TABLE IF EXISTS "Emp";
CREATE TABLE IF NOT EXISTS "Emp" (
	"empno"	INTEGER NOT NULL,
	"ename"	TEXT NOT NULL DEFAULT ('?'),
	"job"	TEXT,
	"mgr"	INTEGER,
	"hiredate"	TEXT,
	"salary"	REAL NOT NULL DEFAULT (100),
	"comm"	REAL DEFAULT (0),
	"deptno"	INTEGER NOT NULL,
	"birthdate"	TEXT NOT NULL DEFAULT ('1960-07-26'),
	"title"	TEXT NOT NULL GENERATED ALWAYS AS ('Jr') STORED,
	"degree"	TEXT NOT NULL DEFAULT (''),
	FOREIGN KEY("deptno") REFERENCES "Dept"("deptno"),
	PRIMARY KEY("empno" AUTOINCREMENT),
	CHECK(("salary" > (COALESCE("comm", 0))))
);
COMMIT;
