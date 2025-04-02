SET SERVEROUTPUT ON;

CREATE OR REPLACE PROCEDURE identify_cars_multiple_accidents IS
    CURSOR car_cursor IS
        SELECT p.regno, COUNT(DISTINCT p.report_no) AS accident_count, SUM(p.damage_amount) AS total_damage
        FROM PARTICIPATED p
        GROUP BY p.regno
        HAVING COUNT(DISTINCT p.report_no) > 1;

    v_car_regno PARTICIPATED.regno%TYPE;
    v_accident_count NUMBER;
    v_total_damage NUMBER(10, 2);

BEGIN
    -- Open the cursor
    OPEN car_cursor;

    -- Loop through all the rows in the cursor
    LOOP
        FETCH car_cursor INTO v_car_regno, v_accident_count, v_total_damage;

        EXIT WHEN car_cursor%NOTFOUND;

        -- Display the car's registration number, accident count, and total damage
        DBMS_OUTPUT.PUT_LINE('Car Registration Number: ' || v_car_regno);
        DBMS_OUTPUT.PUT_LINE('Number of Accidents: ' || v_accident_count);
        DBMS_OUTPUT.PUT_LINE('Total Damage: ' || v_total_damage);
        DBMS_OUTPUT.PUT_LINE('-------------------------------');
    END LOOP;

    -- Close the cursor
    CLOSE car_cursor;

END;
/
