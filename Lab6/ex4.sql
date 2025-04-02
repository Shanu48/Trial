SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE drivers_not_involved_in_accidents IS
BEGIN
    FOR r IN (
        SELECT p.driver_id, p.name, p.address
        FROM PERSON p
        LEFT JOIN PARTICIPATED pt ON p.driver_id = pt.driver_id
        WHERE pt.driver_id IS NULL
    ) LOOP
        DBMS_OUTPUT.PUT_LINE('Driver ID: ' || r.driver_id || ', Name: ' || r.name || ', Address: ' || r.address);
    END LOOP;
END;
/
