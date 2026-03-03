
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
					'jdoe',
					'$2b$12$examplehash1234567890123456789012345678901234567890',
					'John',
					'Doe',
					'+1234567890',
					'john.doe@example.com',
					'1990-05-15 00:00:00',
					'CLIENT'
			) RETURNING id
		)
INSERT INTO "Client" (
	user_id
)
SELECT id FROM new_user;



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
					'alan',
					'$argon2id$v=19$m=19456,t=2,p=1$CGLQqHqeNTLtsw+w1O00cw$awMjsFx02O4mTeDNZx/kDOGIkHVxXcY6aoooeDKEOBA',
					'Alan',
					'Crueger',
					'+1234567890',
					'allan@mildberries.com',
					'1990-05-15 00:00:00',
					'LOGISTITIAN'
			) RETURNING id
