SET SERVEROUTPUT ON;

CREATE OR REPLACE TRIGGER update_total_damage_after_delete
AFTER DELETE ON PARTICIPATED
FOR EACH ROW
DECLARE
    v_total_damage NUMBER(10, 2);
    PRAGMA AUTONOMOUS_TRANSACTION; -- This allows us to perform an independent transaction
BEGIN
    -- Calculate the cumulative damage for the accident report, excluding the deleted damage
    SELECT NVL(SUM(damage_amount), 0)
    INTO v_total_damage
    FROM PARTICIPATED
    WHERE report_no = :OLD.report_no
    GROUP BY report_no;

    -- Update the total_damage in the ACCIDENT table
    UPDATE ACCIDENT
    SET total_damage = v_total_damage
    WHERE report_no = :OLD.report_no;

    -- Commit the changes made to the ACCIDENT table independently
    COMMIT;

END;
/
