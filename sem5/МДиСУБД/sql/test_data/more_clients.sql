WITH new_user AS (
			INSERT INTO "User" (
					login,
					password_hash,
					first_name,
					last_name,
					phone,
					email,
					birth_date,
					role
			) VALUES (
					'asmith',
					'$2b$12$examplehash1234567890123456789012345678901234567890',
					'Alice',
					'Smith',
					'+1987654321',
					'alice.smith@example.com',
					'1985-08-20 00:00:00',
					'CLIENT' 
			) RETURNING id
		)
INSERT INTO "Client" (
	user_id
) 
SELECT id FROM new_user;

WITH new_user AS (
			INSERT INTO "User" (
					login,
					password_hash,
					first_name,
					last_name,
					phone,
					email,
					birth_date,
					role
			) VALUES (
					'bjones',
					'$2b$12$examplehash1234567890123456789012345678901234567890',
					'Bob',
					'Jones',
					'+1122334455',
					'bob.jones@example.com',
					'1992-02-10 00:00:00',
					'CLIENT' 
			) RETURNING id
		)
INSERT INTO "Client" (
	user_id
)
SELECT id FROM new_user;

WITH new_user AS (
			INSERT INTO "User" (
					login,
					password_hash,
					first_name,
					last_name,
					phone,
					email,
					birth_date,
					role
			) VALUES (
					'cwilliam',
					'$2b$12$examplehash1234567890123456789012345678901234567890',
					'Charlie',
					'Williams',
					'+1555666777',
					'charlie.williams@example.com',
					'1988-11-30 00:00:00',
					'CLIENT' 
			) RETURNING id
		)
INSERT INTO "Client" (
	user_id
)
SELECT id FROM new_user;

WITH new_user AS (
			INSERT INTO "User" (
					login,
					password_hash,
					first_name,
					last_name,
					phone,
					email,
					birth_date,
					role
			) VALUES (
					'd.brown',
					'$2b$12$examplehash1234567890123456789012345678901234567890',
					'Diana',
					'Brown',
					'+1444333222',
					'diana.brown@example.com',
					'1995-07-25 00:00:00',
					'CLIENT' 
			) RETURNING id
		)
INSERT INTO "Client" (
	user_id
)
SELECT id FROM new_user;

WITH new_user AS (
			INSERT INTO "User" (
					login,
					password_hash,
					first_name,
					last_name,
					phone,
					email,
					birth_date,
					role
			) VALUES (
					'evan.davis',
					'$2b$12$examplehash1234567890123456789012345678901234567890',
					'Evan',
					'Davis',
					'+1666777888',
					'evan.davis@example.com',
					'1991-09-05 00:00:00',
					'CLIENT' 
			) RETURNING id
		)
INSERT INTO "Client" (
	user_id
)
SELECT id FROM new_user;
