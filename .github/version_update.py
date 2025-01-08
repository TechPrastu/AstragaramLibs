import subprocess
import re
import os

# Function to get the current branch
def get_current_branch():
    try:
        current_branch = os.popen("git rev-parse --abbrev-ref HEAD").read().strip()
        print(f"Current branch: {current_branch}")
        return current_branch
    except Exception as e:
        print(f"Error fetching current branch: {e}")
        return None


# Function to check if there are any tags in the repository
def has_tags():
    try:
        tags = os.popen("git tag").read().strip()
        if not tags:
            return False
        return True
    except Exception as e:
        print(f"Error checking for tags: {e}")
        return False


# Function to get the latest tag
def get_latest_tag():
    try:
        if not has_tags():
            print("No tags found in the repository.")
            return "0.0.0"

        latest_tag_commit = os.popen("git rev-list --tags --max-count=1").read().strip()
        latest_tag = os.popen(f"git describe --tags {latest_tag_commit}").read().strip()
        print(f"Latest tag commit: {latest_tag_commit}")
        print(f"Latest tag result: {latest_tag}")

        if not latest_tag:
            return "0.0.0"

        return latest_tag
    except Exception as e:
        print(f"Error fetching latest tag: {e}")
        return "0.0.0"


# Function to read the current version from the latest tag
def read_current_version():
    latest_tag = get_latest_tag()
    print(f"Current version is: {latest_tag}")
    if not latest_tag:
        return "0.0.0"
    return re.sub(r"\+.*$", "", latest_tag)  # Remove the commit hash part if it exists


# Function to increment the version based on the PR description
def increment_version(current_version, version_type):
    version_parts = list(map(int, current_version.split(".")))
    major, minor, patch = version_parts[0], version_parts[1], version_parts[2]

    if version_type == "Major":
        major += 1
        minor = 0
        patch = 0
    elif version_type == "Minor":
        minor += 1
        patch = 0
    else:
        patch += 1

    new_version = f"{major}.{minor}.{patch}"
    return new_version


# Get the latest commit hash
commit_hash = subprocess.run(
    ["git", "rev-parse", "--short", "HEAD"], capture_output=True, text=True
).stdout.strip()

# Read the current version from the latest tag
current_version = read_current_version()

# Determine the version type from the PR description (default to "Patch")
version_type = "Patch"
pr_description = subprocess.run(
    ["git", "log", "-1", "--pretty=%B"], capture_output=True, text=True
).stdout.strip()
if "[Major]" in pr_description:
    version_type = "Major"
elif "[Minor]" in pr_description:
    version_type = "Minor"

# Increment the version
new_version = increment_version(current_version, version_type)

# Create the full version with the commit hash
full_version = f"{new_version}+{commit_hash}"

# Create the new tag
subprocess.run(
    ["git", "tag", "-a", full_version, "-m", f"Release version {full_version}"]
)
subprocess.run(["git", "push", "origin", full_version])

# Output the new version
print(f"New version is: {full_version}")
