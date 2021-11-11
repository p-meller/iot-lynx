FROM openjdk
COPY MainUnit-0.0.1-SNAPSHOT.jar /app/MainUnit-0.0.1-SNAPSHOT.jar
ENTRYPOINT ["java", "-jar", "/app/MainUnit-0.0.1-SNAPSHOT.jar", "-Dspring.profiles.active=container"]