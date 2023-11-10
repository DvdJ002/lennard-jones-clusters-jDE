# Define constant arguments
$constantArguments = "-N 10 -target -28.422532 -runtimeLmt 15 -Np 100"

# Loop from seed 1 to 50
for ($seed = 1; $seed -le 50; $seed++) {
    # Construct the full command
    $fullCommand = ".\Vaja1.exe $constantArguments -seed $seed"

    # Display the command (optional)
    Write-Host "Running: $fullCommand"

    # Run the command and capture the output
    $output = Invoke-Expression -Command $fullCommand

    # Display the output
    Write-Host ("Output for Seed $seed:")
    Write-Host ($output)
    Write-Host "--------------------------------"
}