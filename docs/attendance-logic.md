# Attendance Logic

## Student Registration

Students are registered in the firmware by adding their scanned UID and display name to the `students` array.

```cpp
const Student students[] = {
  {"04 A3 7B 2C", "Anushka"},
  {"D1 82 5F 19", "Rahul"}
};
```

These are example values. Replace them with actual UIDs from your own cards.

## Duplicate Prevention

The firmware maintains one Boolean flag per registered student. When a student's card is accepted, that flag becomes `true`. Further scans during the same run print `Attendance already marked this session` rather than marking the student again.

The flags are held in RAM, so they reset after a restart or power loss. This project does not persist attendance to flash storage or a database.

## Unknown Cards

If the scanned UID is not found in the registered-student array, the program prints `UID is not registered` and does not mark attendance.

## Security Note

UID-only identification is suitable for a basic classroom prototype, but a UID can sometimes be copied or emulated. Do not treat this design as secure authentication for high-risk access control.
