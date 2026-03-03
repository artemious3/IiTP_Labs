SELECT * FROM "Product"
WHERE to_tsvector('english', title || ' ' || descripion) @@ plainto_tsquery('english', 'alyonka');