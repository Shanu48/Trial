SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE average_damage_per_location IS
    CURSOR location_cursor IS
        SELECT a.location, AVG(p.damage_amount) AS avg_damage
        FROM ACCIDENT a
        JOIN PARTICIPATED p ON a.report_no = p.report_no
        GROUP BY a.location;

    v_location ACCIDENT.location%TYPE;
    v_avg_damage NUMBER(10, 2);

BEGIN
    -- Open the cursor to retrieve the location and average damage
    OPEN location_cursor;

    -- Loop through all the rows in the cursor
    LOOP
        FETCH location_cursor INTO v_location, v_avg_damage;

        EXIT WHEN location_cursor%NOTFOUND;

        -- Display the location and the average damage amount
        DBMS_OUTPUT.PUT_LINE('Location: ' || v_location);
        DBMS_OUTPUT.PUT_LINE('Average Damage: ' || v_avg_damage);
        DBMS_OUTPUT.PUT_LINE('-------------------------------');
    END LOOP;

    -- Close the cursor
    CLOSE location_cursor;

END;
/
