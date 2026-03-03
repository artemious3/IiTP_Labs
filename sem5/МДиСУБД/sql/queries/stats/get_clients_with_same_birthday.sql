SELECT u1.id, u1.first_name, u1.last_name, u2.id, u2.first_name, u2.last_name FROM 
"User" u1, "User" u2
WHERE u1.id != u2.id AND
      u1.birth_date = u2.birth_date;
